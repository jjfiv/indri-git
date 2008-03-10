/*==========================================================================
 * Copyright (c) 2006 University of Massachusetts.  All Rights Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval
 * is subject to the terms of the software license set forth in the LICENSE
 * file included with this software, and also available at
 * http://www.lemurproject.org/license.html
 *
 *==========================================================================
*/
/*
        Force aggregation of the component libraries into a unified indri.lib.
*/
#ifdef WIN32
#pragma comment (lib, "zlib.lib")
#pragma comment (lib, "xpdf.lib")
#pragma comment (lib, "antlr.lib")
#pragma comment (lib, "lemur.lib")
#pragma comment (lib, "indri.lib")
#endif
