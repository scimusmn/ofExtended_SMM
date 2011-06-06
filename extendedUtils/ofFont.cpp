/*
 *  ofFont.cpp
 *  openFrameworksLib
 *
 *  Created by Exhibits on 3/10/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "ofFont.h"

vector<ofTrueTypeFont> fonts;
map<int,int> points;
map<string,map<int,ofTrueTypeFont> > Fonts;

//typedef map<string, map<int,ofTrueTypeFont> > fontMap;

string ssprintf(const char * format, ...)
{
	int amount;
	string temp=format;
	size_t found;
	found=temp.find("%");
	amount++;
	while (found!=string::npos) {
		found=temp.find("%",found+1);
		amount++;
	}
	found=0;
	char tmp[1024];
	va_list vl;
	va_start(vl,amount);
	vsprintf(tmp, format, vl);
	va_end(vl);
	temp=tmp;
	return temp;
}

void ofFontContainer::setSize(int pt){
	map<int,ofTrueTypeFont>::iterator it=Fonts[file].find(pt);
	if (it==Fonts[file].end()) {
		Fonts[file][pt].loadFont(file,pt);
		size=Fonts[file].find(pt);
	}
	else {
		size=it;
	}
}

void ofFontContainer::loadFont(string filename, int pt){
	file=filename;
	map<string,map<int,ofTrueTypeFont> >::iterator it=Fonts.find(filename);
	if (it==Fonts.end()) {
		Fonts[filename][12]=ofTrueTypeFont();
		Fonts[filename][12].loadFont(filename,12);
		font=Fonts.find(filename);
	}
	else {
		font=it;
	}
	if (pt) {
		setSize(pt);
	}
}

void ofFontContainer::operator=(const ofFontContainer & t)
{
	font=t.font;
	size=t.size;
	file=t.file;
}

ofTrueTypeFont & ofFontContainer::operator[](int i)
{
	return size->second;
}

void ofFontContainer::drawString(string s, int x, int y)
{
	size->second.drawString(s,x,y);
}

ofRectangle ofFontContainer::getBoundingBox(string s, int x, int y)
{
	return size->second.getStringBoundingBox(s,x,y);
}

float ofFontContainer::stringWidth(string s)
{
	return size->second.stringWidth(s);
}

float ofFontContainer::stringHeight(string s)
{
	return size->second.stringHeight(s);
}

//map<string,ofFontContainer> font;


ofFont::ofFont()
{
	fonts.reserve(100);
	FONT_MODE=OF_FONT_LEFT;
	V_FONT_MODE=OF_FONT_BOT;
}

void ofFont::setMode(ofFontMode mode){
	if(mode<4) FONT_MODE=mode;
	else V_FONT_MODE=mode;
}

void ofFont::loadFont(string filename)
{
	file=filename;
	setNum=checkSizes(12);
}

void ofFont::setSize(int pt)
{
	setNum=checkSizes(pt);
}

int ofFont::checkSizes(int pt)
{
	point=pt;
	int ret=0;
	if(pt>=0){
		map<int,int>::iterator it=points.find(pt);
		if (it==points.end()) {
			int current=fonts.size();
			fonts.push_back(ofTrueTypeFont());
			fonts[current].loadFont(file,pt);
			ret=points[pt]=current;
		}
		else {
			ret=it->second;
		}
	}
	else {
		ret=setNum;
	}
	
	return ret;
}

void ofFont::drawString(string s, int x, int y)
{
	if(FONT_MODE==OF_FONT_CENTER) x=(x-stringWidth(s)/2);
	else if(FONT_MODE==OF_FONT_RIGHT) x=(x-stringWidth(s));
	if(V_FONT_MODE==OF_FONT_MID) y=(y+stringHeight("Kj")/2);
	else if(V_FONT_MODE==OF_FONT_TOP) y=(y+stringHeight("K"));
	fonts[setNum].drawString(s,x,y);
}

ofRectangle ofFont::getBoundingBox(string s, int x, int y)
{
	if(FONT_MODE==OF_FONT_CENTER) x=(x-stringWidth(s)/2);
	else if(FONT_MODE==OF_FONT_RIGHT) x=(x-stringWidth(s));
	return fonts[setNum].getStringBoundingBox(s,x,y);
}

float 		ofFont::stringWidth(string s)
{
	return fonts[setNum].stringWidth(s);
}

float 		ofFont::stringHeight(string s)
{
	return fonts[setNum].stringHeight(s);
}