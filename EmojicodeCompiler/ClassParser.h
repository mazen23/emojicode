//
//  ClassParser.h
//  Emojicode
//
//  Created by Theo Weidmann on 05.03.15.
//  Copyright (c) 2015 Theo Weidmann. All rights reserved.
//

#ifndef __Emojicode__ClassParser__
#define __Emojicode__ClassParser__

#include "EmojicodeCompiler.h"

/**
 * The class parsers reads the source code file that only consists of type definitions (class and protocols).
 */

/**
 * Parses the given file.
 * @param path Path to the file.
 * @param pkg The name of the package being parsed.
 * @param allowNative Whether classes’s functions may be implemented using native code. Should be @c true when parsing a package file.
 */
void parseFile(const char *path, Package *pkg, bool allowNative, EmojicodeChar namespace);

void saveBlock(Procedure *p, bool allowNative);


/**
 * Check whether an override or an implementation of a method or initializer is breaking promises.
 * @param type A string to describe the "super method" or "definition"
 */
void checkPromises(Procedure *sub, Procedure *super, const char *type, Type parentType);

void parseArgumentList(Procedure *pc);

void parseReturnType(Type *type, Class *class, EmojicodeChar theNamespace);

#endif /* defined(__Emojicode__ClassParser__) */