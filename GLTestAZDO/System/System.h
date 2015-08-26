//
//  System.h
//  ion
//
//  Created by Claire Rogers on 14/10/2012.
//  Copyright (c) 2013 Guilty Dog Productions Ltd. All rights reserved.
//

#pragma once 

void AssertFunc(const char *, const char *, int, const char *);
void AssertMsgFunc(const char *func, const char *file, int line, const char *fmt, ...);
			
#define AssertMsg(e, fmt, ...)	((!e) ? AssertMsgFunc(__FUNCTION__, __FILE__, __LINE__, fmt, ##__VA_ARGS__) : (void)0)
#define	Assert(e)				((!e) ? AssertFunc(__FUNCTION__, __FILE__, __LINE__, #e) : (void)0)


