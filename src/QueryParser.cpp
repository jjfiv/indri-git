/* $ANTLR 2.7.4: "indrilang.g" -> "QueryParser.cpp"$ */
#include "indri/QueryParser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
ANTLR_BEGIN_NAMESPACE(indri)
ANTLR_BEGIN_NAMESPACE(lang)
#line 1 "indrilang.g"
#line 8 "QueryParser.cpp"
QueryParser::QueryParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

QueryParser::QueryParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,1)
{
}

QueryParser::QueryParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

QueryParser::QueryParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,1)
{
}

QueryParser::QueryParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,1)
{
}

 indri::lang::ScoredExtentNode*  QueryParser::query() {
#line 167 "indrilang.g"
	 indri::lang::ScoredExtentNode* q ;
#line 37 "QueryParser.cpp"
#line 167 "indrilang.g"
	
	indri::lang::CombineNode* c = 0;
	indri::lang::ScoredExtentNode* s = 0;
	q = 0;
	
#line 44 "QueryParser.cpp"
	
	q=scoredExtentNode();
	{
	switch ( LA(1)) {
	case WSUM:
	case WAND:
	case OD:
	case OR:
	case NOT:
	case UW:
	case COMBINE:
	case WEIGHT:
	case MAX:
	case FILREQ:
	case FILREJ:
	case ANY:
	case BAND:
	case SYN:
	case PRIOR:
	case DATEAFTER:
	case DATEBEFORE:
	case DATEBETWEEN:
	case LESS:
	case GREATER:
	case BETWEEN:
	case EQUALS:
	case NUMBER:
	case FLOAT:
	case O_ANGLE:
	case O_BRACE:
	case DBL_QUOTE:
	case TERM:
	case ENCODED_QUOTED_TERM:
	case ENCODED_TERM:
	case OPERATOR:
	{
		s=scoredExtentNode();
		if ( inputState->guessing==0 ) {
#line 174 "indrilang.g"
			
			c = new CombineNode;
			c->addChild(q);
			c->addChild(s);
			_nodes.push_back(c);
			q = c;
			
#line 91 "QueryParser.cpp"
		}
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				s=scoredExtentNode();
				if ( inputState->guessing==0 ) {
#line 182 "indrilang.g"
					
					c->addChild(s);
					
#line 102 "QueryParser.cpp"
				}
			}
			else {
				goto _loop82;
			}
			
		}
		_loop82:;
		} // ( ... )*
		break;
	}
	case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
	return q ;
}

 indri::lang::ScoredExtentNode*  QueryParser::scoredExtentNode() {
#line 188 "indrilang.g"
	 indri::lang::ScoredExtentNode* s ;
#line 131 "QueryParser.cpp"
	
	switch ( LA(1)) {
	case WEIGHT:
	{
		s=weightNode();
		break;
	}
	case COMBINE:
	{
		s=combineNode();
		break;
	}
	case OR:
	{
		s=orNode();
		break;
	}
	case NOT:
	{
		s=notNode();
		break;
	}
	case WAND:
	{
		s=wandNode();
		break;
	}
	case WSUM:
	{
		s=wsumNode();
		break;
	}
	case MAX:
	{
		s=maxNode();
		break;
	}
	case PRIOR:
	{
		s=priorNode();
		break;
	}
	case FILREJ:
	{
		s=filrejNode();
		break;
	}
	case FILREQ:
	{
		s=filreqNode();
		break;
	}
	case OD:
	case UW:
	case ANY:
	case BAND:
	case SYN:
	case DATEAFTER:
	case DATEBEFORE:
	case DATEBETWEEN:
	case LESS:
	case GREATER:
	case BETWEEN:
	case EQUALS:
	case NUMBER:
	case FLOAT:
	case O_ANGLE:
	case O_BRACE:
	case DBL_QUOTE:
	case TERM:
	case ENCODED_QUOTED_TERM:
	case ENCODED_TERM:
	case OPERATOR:
	{
		s=scoredRaw();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return s ;
}

 indri::lang::ScoredExtentNode*  QueryParser::weightNode() {
#line 284 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 220 "QueryParser.cpp"
#line 284 "indrilang.g"
	
	indri::lang::WeightNode* wn = new indri::lang::WeightNode;
	_nodes.push_back(wn);
	
#line 226 "QueryParser.cpp"
	
	match(WEIGHT);
	r=weightedList(wn);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::combineNode() {
#line 291 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 236 "QueryParser.cpp"
#line 291 "indrilang.g"
	
	indri::lang::CombineNode* cn = new indri::lang::CombineNode;
	_nodes.push_back(cn);
	
#line 242 "QueryParser.cpp"
	
	match(COMBINE);
	r=unweightedList(cn);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::orNode() {
#line 319 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 252 "QueryParser.cpp"
#line 319 "indrilang.g"
	
	indri::lang::OrNode* on = new indri::lang::OrNode;
	_nodes.push_back(on);
	
#line 258 "QueryParser.cpp"
	
	match(OR);
	r=unweightedList(on);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::notNode() {
#line 333 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 268 "QueryParser.cpp"
#line 333 "indrilang.g"
	
	indri::lang::NotNode* n = new indri::lang::NotNode;
	indri::lang::ScoredExtentNode* c = 0;
	_nodes.push_back(n);
	r = n;
	
#line 276 "QueryParser.cpp"
	
	match(NOT);
	{
	switch ( LA(1)) {
	case O_SQUARE:
	{
		r=extentRestriction(r);
		break;
	}
	case O_PAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(O_PAREN);
	c=scoredExtentNode();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 341 "indrilang.g"
		
		n->setChild(c);
		
#line 304 "QueryParser.cpp"
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::wandNode() {
#line 312 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 312 "QueryParser.cpp"
#line 312 "indrilang.g"
	
	indri::lang::WAndNode* wn = new indri::lang::WAndNode;
	_nodes.push_back(wn);
	
#line 318 "QueryParser.cpp"
	
	match(WAND);
	r=weightedList(wn);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::wsumNode() {
#line 305 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 328 "QueryParser.cpp"
#line 305 "indrilang.g"
	
	indri::lang::WSumNode* wn = new indri::lang::WSumNode;
	_nodes.push_back(wn);
	
#line 334 "QueryParser.cpp"
	
	match(WSUM);
	r=weightedList(wn);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::maxNode() {
#line 326 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 344 "QueryParser.cpp"
#line 326 "indrilang.g"
	
	indri::lang::MaxNode* mn = new indri::lang::MaxNode;
	_nodes.push_back(mn);
	
#line 350 "QueryParser.cpp"
	
	match(MAX);
	r=unweightedList(mn);
	return r ;
}

 indri::lang::PriorNode*  QueryParser::priorNode() {
#line 345 "indrilang.g"
	 indri::lang::PriorNode* p ;
#line 360 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  name = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 345 "indrilang.g"
	
	indri::lang::Field* field = 0;
	p = 0;
	
#line 367 "QueryParser.cpp"
	
	match(PRIOR);
	match(O_PAREN);
	name = LT(1);
	match(TERM);
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 350 "indrilang.g"
		
		p = _priorFactory->create( name->getText() );
		
		if( p->getFieldName().length() ) {
		field = new Field( p->getFieldName() );
		p->setField( field );
		_nodes.push_back(field);
		}
		
		_nodes.push_back(p);
		
#line 387 "QueryParser.cpp"
	}
	return p ;
}

 indri::lang::FilRejNode*  QueryParser::filrejNode() {
#line 421 "indrilang.g"
	 indri::lang::FilRejNode* fj ;
#line 395 "QueryParser.cpp"
#line 421 "indrilang.g"
	
	RawExtentNode* filter = 0;
	ScoredExtentNode* disallowed = 0;
	
#line 401 "QueryParser.cpp"
	
	match(FILREJ);
	match(O_PAREN);
	filter=unscoredTerm();
	disallowed=scoredExtentNode();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 427 "indrilang.g"
		
		fj = new FilRejNode( filter, disallowed );
		_nodes.push_back(fj);
		
#line 414 "QueryParser.cpp"
	}
	return fj ;
}

 indri::lang::FilReqNode*  QueryParser::filreqNode() {
#line 432 "indrilang.g"
	 indri::lang::FilReqNode* fq ;
#line 422 "QueryParser.cpp"
#line 432 "indrilang.g"
	
	RawExtentNode* filter = 0;
	ScoredExtentNode* required = 0;
	
#line 428 "QueryParser.cpp"
	
	match(FILREQ);
	match(O_PAREN);
	filter=unscoredTerm();
	required=scoredExtentNode();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 438 "indrilang.g"
		
		fq = new FilReqNode( filter, required );
		_nodes.push_back(fq);
		
#line 441 "QueryParser.cpp"
	}
	return fq ;
}

 ScoredExtentNode*  QueryParser::scoredRaw() {
#line 207 "indrilang.g"
	 ScoredExtentNode* sn ;
#line 449 "QueryParser.cpp"
#line 207 "indrilang.g"
	
	RawExtentNode* raw = 0;
	RawExtentNode* contexts = 0;
	sn = 0;
	
#line 456 "QueryParser.cpp"
	
	bool synPredMatched106 = false;
	if (((_tokenSet_1.member(LA(1))))) {
		int _m106 = mark();
		synPredMatched106 = true;
		inputState->guessing++;
		try {
			{
			qualifiedTerm();
			context_list();
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched106 = false;
		}
		rewind(_m106);
		inputState->guessing--;
	}
	if ( synPredMatched106 ) {
		raw=qualifiedTerm();
		contexts=context_list();
		if ( inputState->guessing==0 ) {
#line 214 "indrilang.g"
			
			sn = new indri::lang::RawScorerNode( raw, contexts );
			_nodes.push_back(sn);
			
#line 484 "QueryParser.cpp"
		}
	}
	else {
		bool synPredMatched108 = false;
		if (((_tokenSet_1.member(LA(1))))) {
			int _m108 = mark();
			synPredMatched108 = true;
			inputState->guessing++;
			try {
				{
				qualifiedTerm();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched108 = false;
			}
			rewind(_m108);
			inputState->guessing--;
		}
		if ( synPredMatched108 ) {
			raw=qualifiedTerm();
			if ( inputState->guessing==0 ) {
#line 219 "indrilang.g"
				
				sn = new indri::lang::RawScorerNode( raw, contexts );
				_nodes.push_back(sn);
				
#line 512 "QueryParser.cpp"
			}
		}
		else {
			bool synPredMatched110 = false;
			if (((_tokenSet_1.member(LA(1))))) {
				int _m110 = mark();
				synPredMatched110 = true;
				inputState->guessing++;
				try {
					{
					unqualifiedTerm();
					match(DOT);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched110 = false;
				}
				rewind(_m110);
				inputState->guessing--;
			}
			if ( synPredMatched110 ) {
				raw=unqualifiedTerm();
				match(DOT);
				contexts=context_list();
				if ( inputState->guessing==0 ) {
#line 224 "indrilang.g"
					
					sn = new indri::lang::RawScorerNode( raw, contexts );
					_nodes.push_back(sn);
					
#line 543 "QueryParser.cpp"
				}
			}
			else if ((_tokenSet_1.member(LA(1)))) {
				raw=unqualifiedTerm();
				if ( inputState->guessing==0 ) {
#line 229 "indrilang.g"
					
					sn = new indri::lang::RawScorerNode( raw, contexts );
					_nodes.push_back(sn);
					
#line 554 "QueryParser.cpp"
				}
			}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}}
	return sn ;
}

 RawExtentNode*  QueryParser::qualifiedTerm() {
#line 459 "indrilang.g"
	 RawExtentNode* t ;
#line 567 "QueryParser.cpp"
#line 459 "indrilang.g"
	
	RawExtentNode* synonyms = 0;
	RawExtentNode* fields = 0;
	t = 0;
	
#line 574 "QueryParser.cpp"
	
	synonyms=unqualifiedTerm();
	match(DOT);
	fields=field_list();
	if ( inputState->guessing==0 ) {
#line 466 "indrilang.g"
		
		if( fields ) {
		t = new indri::lang::ExtentInside( synonyms, fields );
		_nodes.push_back(t);
		} else {
		t = synonyms;
		}
		
#line 589 "QueryParser.cpp"
	}
	return t ;
}

 ExtentOr*  QueryParser::context_list() {
#line 567 "indrilang.g"
	 ExtentOr* contexts ;
#line 597 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  first = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  additional = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 567 "indrilang.g"
	
	contexts = new ExtentOr;
	_nodes.push_back( contexts );
	
#line 605 "QueryParser.cpp"
	
	match(O_PAREN);
	first = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 573 "indrilang.g"
		
		Field* firstField = new indri::lang::Field( first->getText() );
		_nodes.push_back( firstField );
		contexts->addChild( firstField );
		
#line 617 "QueryParser.cpp"
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			additional = LT(1);
			match(TERM);
			if ( inputState->guessing==0 ) {
#line 580 "indrilang.g"
				
				Field* additionalField = new Field(additional->getText());
				_nodes.push_back( additionalField );
				contexts->addChild( additionalField );
				
#line 632 "QueryParser.cpp"
			}
		}
		else {
			goto _loop208;
		}
		
	}
	_loop208:;
	} // ( ... )*
	match(C_PAREN);
	return contexts ;
}

 RawExtentNode*  QueryParser::unqualifiedTerm() {
#line 475 "indrilang.g"
	 RawExtentNode* re ;
#line 649 "QueryParser.cpp"
	
	switch ( LA(1)) {
	case OD:
	case OPERATOR:
	{
		re=odNode();
		break;
	}
	case UW:
	{
		re=uwNode();
		break;
	}
	case BAND:
	{
		re=bandNode();
		break;
	}
	case DATEBEFORE:
	{
		re=dateBefore();
		break;
	}
	case DATEAFTER:
	{
		re=dateAfter();
		break;
	}
	case DATEBETWEEN:
	{
		re=dateBetween();
		break;
	}
	case O_ANGLE:
	{
		re=synonym_list();
		break;
	}
	case O_BRACE:
	{
		re=synonym_list_brace();
		break;
	}
	case SYN:
	{
		re=synonym_list_alt();
		break;
	}
	case ANY:
	{
		re=anyField();
		break;
	}
	case LESS:
	{
		re=lessNode();
		break;
	}
	case GREATER:
	{
		re=greaterNode();
		break;
	}
	case BETWEEN:
	{
		re=betweenNode();
		break;
	}
	case EQUALS:
	{
		re=equalsNode();
		break;
	}
	case NUMBER:
	case FLOAT:
	case DBL_QUOTE:
	case TERM:
	case ENCODED_QUOTED_TERM:
	case ENCODED_TERM:
	{
		re=rawText();
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return re ;
}

 indri::lang::ScoredExtentNode*  QueryParser::weightedList(
	 indri::lang::WeightedCombinationNode* wn 
) {
#line 245 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 746 "QueryParser.cpp"
#line 245 "indrilang.g"
	
	double w = 0;
	ScoredExtentNode* n = 0;
	sr = wn;
	
#line 753 "QueryParser.cpp"
	
	{
	switch ( LA(1)) {
	case O_SQUARE:
	{
		sr=extentRestriction(wn);
		break;
	}
	case O_PAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(O_PAREN);
	{ // ( ... )+
	int _cnt114=0;
	for (;;) {
		if ((LA(1) == NUMBER || LA(1) == FLOAT)) {
			w=floating();
			n=scoredExtentNode();
			if ( inputState->guessing==0 ) {
#line 256 "indrilang.g"
				wn->addChild( w, n );
#line 782 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt114>=1 ) { goto _loop114; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt114++;
	}
	_loop114:;
	}  // ( ... )+
	match(C_PAREN);
	return sr ;
}

 indri::lang::ScoredExtentNode*  QueryParser::extentRestriction(
	 indri::lang::ScoredExtentNode* sn 
) {
#line 492 "indrilang.g"
	 indri::lang::ScoredExtentNode* er ;
#line 802 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  passageWindowSize = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  inc = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 492 "indrilang.g"
	
	indri::lang::Field* f = 0;
	er = 0;
	
#line 811 "QueryParser.cpp"
	
	bool synPredMatched193 = false;
	if (((LA(1) == O_SQUARE))) {
		int _m193 = mark();
		synPredMatched193 = true;
		inputState->guessing++;
		try {
			{
			match(O_SQUARE);
			match(TERM);
			match(COLON);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched193 = false;
		}
		rewind(_m193);
		inputState->guessing--;
	}
	if ( synPredMatched193 ) {
		match(O_SQUARE);
		passageWindowSize = LT(1);
		match(TERM);
		match(COLON);
		inc = LT(1);
		match(NUMBER);
		match(C_SQUARE);
		if ( inputState->guessing==0 ) {
#line 497 "indrilang.g"
			
			int startWindow;
			
			for( startWindow = 0; startWindow < passageWindowSize->getText().size(); startWindow++ ) {
			if( isdigit( passageWindowSize->getText()[startWindow] ) )
			break;
			}
			
			int increment = atoi(inc->getText().c_str());
			int windowSize = atoi(passageWindowSize->getText().c_str() + startWindow );
			
			er = new indri::lang::FixedPassage(sn, windowSize, increment);
			
#line 854 "QueryParser.cpp"
		}
	}
	else if ((LA(1) == O_SQUARE)) {
		match(O_SQUARE);
		match(LITERAL_passage);
		field = LT(1);
		match(TERM);
		match(C_SQUARE);
		if ( inputState->guessing==0 ) {
#line 511 "indrilang.g"
			
			f = new indri::lang::Field(field->getText());
			_nodes.push_back(f);
			er = new indri::lang::ExtentRestriction(sn, f);
			_nodes.push_back(er);
			
#line 871 "QueryParser.cpp"
		}
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return er ;
}

 double  QueryParser::floating() {
#line 717 "indrilang.g"
	 double d ;
#line 884 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 717 "indrilang.g"
	
	d = 0;
	
#line 891 "QueryParser.cpp"
	
	switch ( LA(1)) {
	case FLOAT:
	{
		f = LT(1);
		match(FLOAT);
		if ( inputState->guessing==0 ) {
#line 720 "indrilang.g"
			
			d = atof(f->getText().c_str());
			
#line 903 "QueryParser.cpp"
		}
		break;
	}
	case NUMBER:
	{
		n = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 723 "indrilang.g"
			
			d = atof(n->getText().c_str());
			
#line 916 "QueryParser.cpp"
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return d ;
}

 indri::lang::ScoredExtentNode*  QueryParser::sumList(
	 indri::lang::WSumNode* wn 
) {
#line 261 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 933 "QueryParser.cpp"
#line 261 "indrilang.g"
	
	double w = 0;
	ScoredExtentNode* n = 0;
	sr = wn;
	
#line 940 "QueryParser.cpp"
	
	{
	switch ( LA(1)) {
	case O_SQUARE:
	{
		sr=extentRestriction(wn);
		break;
	}
	case O_PAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(O_PAREN);
	{ // ( ... )+
	int _cnt118=0;
	for (;;) {
		if ((_tokenSet_0.member(LA(1)))) {
			n=scoredExtentNode();
			if ( inputState->guessing==0 ) {
#line 269 "indrilang.g"
				wn->addChild( 1.0, n );
#line 968 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt118>=1 ) { goto _loop118; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt118++;
	}
	_loop118:;
	}  // ( ... )+
	match(C_PAREN);
	return sr ;
}

 indri::lang::ScoredExtentNode*  QueryParser::unweightedList(
	 indri::lang::UnweightedCombinationNode* cn 
) {
#line 273 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 988 "QueryParser.cpp"
#line 273 "indrilang.g"
	
	ScoredExtentNode* n = 0;
	sr = cn;
	
#line 994 "QueryParser.cpp"
	
	{
	switch ( LA(1)) {
	case O_SQUARE:
	{
		sr=extentRestriction(cn);
		break;
	}
	case O_PAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(O_PAREN);
	{ // ( ... )+
	int _cnt122=0;
	for (;;) {
		if ((_tokenSet_0.member(LA(1)))) {
			n=scoredExtentNode();
			if ( inputState->guessing==0 ) {
#line 280 "indrilang.g"
				cn->addChild( n );
#line 1022 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt122>=1 ) { goto _loop122; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt122++;
	}
	_loop122:;
	}  // ( ... )+
	match(C_PAREN);
	return sr ;
}

 indri::lang::ScoredExtentNode*  QueryParser::sumNode() {
#line 298 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 1040 "QueryParser.cpp"
#line 298 "indrilang.g"
	
	indri::lang::WSumNode* wn = new indri::lang::WSumNode;
	_nodes.push_back(wn);
	
#line 1046 "QueryParser.cpp"
	
	match(SUM);
	r=sumList(wn);
	return r ;
}

 indri::lang::ODNode*  QueryParser::odNode() {
#line 372 "indrilang.g"
	 indri::lang::ODNode* od ;
#line 1056 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 372 "indrilang.g"
	
	RawExtentNode* rn = 0;
	od = new indri::lang::ODNode;
	_nodes.push_back(od);
	
#line 1065 "QueryParser.cpp"
	
	{
	bool synPredMatched136 = false;
	if (((LA(1) == OD))) {
		int _m136 = mark();
		synPredMatched136 = true;
		inputState->guessing++;
		try {
			{
			match(OD);
			match(NUMBER);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched136 = false;
		}
		rewind(_m136);
		inputState->guessing--;
	}
	if ( synPredMatched136 ) {
		{
		match(OD);
		n1 = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 381 "indrilang.g"
			od->setWindowSize( n1->getText() );
#line 1093 "QueryParser.cpp"
		}
		}
	}
	else {
		bool synPredMatched139 = false;
		if (((LA(1) == OD))) {
			int _m139 = mark();
			synPredMatched139 = true;
			inputState->guessing++;
			try {
				{
				match(OD);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched139 = false;
			}
			rewind(_m139);
			inputState->guessing--;
		}
		if ( synPredMatched139 ) {
			{
			match(OD);
			}
		}
		else if ((LA(1) == OPERATOR)) {
			{
			match(OPERATOR);
			n2 = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 385 "indrilang.g"
				od->setWindowSize( n2->getText() );
#line 1127 "QueryParser.cpp"
			}
			}
		}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(O_PAREN);
	{ // ( ... )+
	int _cnt143=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			rn=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 389 "indrilang.g"
				od->addChild( rn );
#line 1145 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt143>=1 ) { goto _loop143; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt143++;
	}
	_loop143:;
	}  // ( ... )+
	match(C_PAREN);
	return od ;
}

 RawExtentNode*  QueryParser::unscoredTerm() {
#line 452 "indrilang.g"
	 RawExtentNode* t ;
#line 1163 "QueryParser.cpp"
#line 452 "indrilang.g"
	
	t = 0;
	
#line 1168 "QueryParser.cpp"
	
	bool synPredMatched160 = false;
	if (((_tokenSet_1.member(LA(1))))) {
		int _m160 = mark();
		synPredMatched160 = true;
		inputState->guessing++;
		try {
			{
			qualifiedTerm();
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched160 = false;
		}
		rewind(_m160);
		inputState->guessing--;
	}
	if ( synPredMatched160 ) {
		t=qualifiedTerm();
	}
	else if ((_tokenSet_1.member(LA(1)))) {
		t=unqualifiedTerm();
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return t ;
}

 indri::lang::UWNode*  QueryParser::uwNode() {
#line 393 "indrilang.g"
	 indri::lang::UWNode* uw ;
#line 1202 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 393 "indrilang.g"
	
	uw = new indri::lang::UWNode;
	RawExtentNode* rn = 0;
	_nodes.push_back(uw);
	
#line 1210 "QueryParser.cpp"
	
	{
	bool synPredMatched147 = false;
	if (((LA(1) == UW))) {
		int _m147 = mark();
		synPredMatched147 = true;
		inputState->guessing++;
		try {
			{
			match(UW);
			match(NUMBER);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched147 = false;
		}
		rewind(_m147);
		inputState->guessing--;
	}
	if ( synPredMatched147 ) {
		{
		match(UW);
		n = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 401 "indrilang.g"
			uw->setWindowSize( n->getText() );
#line 1238 "QueryParser.cpp"
		}
		}
	}
	else if ((LA(1) == UW)) {
		{
		match(UW);
		}
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	}
	match(O_PAREN);
	{ // ( ... )+
	int _cnt151=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			rn=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 407 "indrilang.g"
				uw->addChild( rn );
#line 1261 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt151>=1 ) { goto _loop151; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt151++;
	}
	_loop151:;
	}  // ( ... )+
	match(C_PAREN);
	return uw ;
}

 indri::lang::BAndNode*  QueryParser::bandNode() {
#line 410 "indrilang.g"
	 indri::lang::BAndNode* b ;
#line 1279 "QueryParser.cpp"
#line 410 "indrilang.g"
	
	b = new indri::lang::BAndNode;
	RawExtentNode* rn = 0;
	_nodes.push_back(b);
	
#line 1286 "QueryParser.cpp"
	
	match(BAND);
	match(O_PAREN);
	{ // ( ... )+
	int _cnt154=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			rn=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 418 "indrilang.g"
				b->addChild( rn );
#line 1298 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt154>=1 ) { goto _loop154; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt154++;
	}
	_loop154:;
	}  // ( ... )+
	match(C_PAREN);
	return b ;
}

 indri::lang::Field*  QueryParser::anyField() {
#line 443 "indrilang.g"
	 indri::lang::Field* f ;
#line 1316 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 443 "indrilang.g"
	
	f = 0;
	
#line 1322 "QueryParser.cpp"
	
	match(ANY);
	match(COLON);
	t = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 447 "indrilang.g"
		
		f = new Field(t->getText());
		_nodes.push_back(f);
		
#line 1334 "QueryParser.cpp"
	}
	return f ;
}

 ExtentAnd*  QueryParser::field_list() {
#line 547 "indrilang.g"
	 ExtentAnd* fields ;
#line 1342 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  first = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  additional = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 547 "indrilang.g"
	
	fields = new ExtentAnd;
	_nodes.push_back( fields );
	
#line 1350 "QueryParser.cpp"
	
	first = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 553 "indrilang.g"
		
		Field* firstField = new indri::lang::Field( first->getText() );
		_nodes.push_back( firstField );
		fields->addChild( firstField );
		
#line 1361 "QueryParser.cpp"
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			additional = LT(1);
			match(TERM);
			if ( inputState->guessing==0 ) {
#line 560 "indrilang.g"
				
				Field* additionalField = new Field(additional->getText());
				_nodes.push_back( additionalField );
				fields->addChild( additionalField );
				
#line 1376 "QueryParser.cpp"
			}
		}
		else {
			goto _loop205;
		}
		
	}
	_loop205:;
	} // ( ... )*
	return fields ;
}

 FieldLessNode*  QueryParser::dateBefore() {
#line 596 "indrilang.g"
	 FieldLessNode* extent ;
#line 1392 "QueryParser.cpp"
#line 596 "indrilang.g"
	
	UINT64 d = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1399 "QueryParser.cpp"
	
	match(DATEBEFORE);
	match(O_PAREN);
	d=date();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 601 "indrilang.g"
		
		dateField = new Field("date");
		extent = new FieldLessNode( dateField, d );
		_nodes.push_back( dateField );
		_nodes.push_back( extent );
		
#line 1413 "QueryParser.cpp"
	}
	return extent ;
}

 FieldGreaterNode*  QueryParser::dateAfter() {
#line 608 "indrilang.g"
	 FieldGreaterNode* extent ;
#line 1421 "QueryParser.cpp"
#line 608 "indrilang.g"
	
	UINT64 d = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1428 "QueryParser.cpp"
	
	match(DATEAFTER);
	match(O_PAREN);
	d=date();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 613 "indrilang.g"
		
		dateField = new Field("date");
		extent = new FieldGreaterNode( dateField, d );
		_nodes.push_back( dateField );
		_nodes.push_back( extent );
		
#line 1442 "QueryParser.cpp"
	}
	return extent ;
}

 FieldBetweenNode*  QueryParser::dateBetween() {
#line 620 "indrilang.g"
	 FieldBetweenNode* extent ;
#line 1450 "QueryParser.cpp"
#line 620 "indrilang.g"
	
	UINT64 low = 0;
	UINT64 high = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1458 "QueryParser.cpp"
	
	match(DATEBETWEEN);
	match(O_PAREN);
	low=date();
	match(COMMA);
	high=date();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 626 "indrilang.g"
		
		dateField = new Field("date");
		extent = new FieldBetweenNode( dateField, low, high );
		_nodes.push_back( dateField );
		_nodes.push_back( extent );
		
#line 1474 "QueryParser.cpp"
	}
	return extent ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list() {
#line 518 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1482 "QueryParser.cpp"
#line 518 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1489 "QueryParser.cpp"
	
	match(O_ANGLE);
	{ // ( ... )+
	int _cnt196=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			term=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 524 "indrilang.g"
				s->addChild(term);
#line 1500 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt196>=1 ) { goto _loop196; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt196++;
	}
	_loop196:;
	}  // ( ... )+
	match(C_ANGLE);
	return s ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list_brace() {
#line 527 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1518 "QueryParser.cpp"
#line 527 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1525 "QueryParser.cpp"
	
	match(O_BRACE);
	{ // ( ... )+
	int _cnt199=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			term=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 533 "indrilang.g"
				s->addChild(term);
#line 1536 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt199>=1 ) { goto _loop199; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt199++;
	}
	_loop199:;
	}  // ( ... )+
	match(C_BRACE);
	return s ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list_alt() {
#line 536 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1554 "QueryParser.cpp"
#line 536 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	// semantics of this node will change
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1562 "QueryParser.cpp"
	
	match(SYN);
	match(O_PAREN);
	{ // ( ... )+
	int _cnt202=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			term=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 544 "indrilang.g"
				s->addChild(term);
#line 1574 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt202>=1 ) { goto _loop202; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt202++;
	}
	_loop202:;
	}  // ( ... )+
	match(C_PAREN);
	return s ;
}

 FieldLessNode*  QueryParser::lessNode() {
#line 749 "indrilang.g"
	 FieldLessNode* ln ;
#line 1592 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 749 "indrilang.g"
	
	ln = 0;
	Field* compareField = 0;
	INT64 high = 0;
	
#line 1600 "QueryParser.cpp"
	
	match(LESS);
	match(O_PAREN);
	field = LT(1);
	match(TERM);
	high=number();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 754 "indrilang.g"
		
		compareField = new Field(field->getText());
		ln = new FieldLessNode( compareField, high );
		_nodes.push_back( compareField );
		_nodes.push_back( ln );
		
#line 1616 "QueryParser.cpp"
	}
	return ln ;
}

 FieldGreaterNode*  QueryParser::greaterNode() {
#line 737 "indrilang.g"
	 FieldGreaterNode* gn ;
#line 1624 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 737 "indrilang.g"
	
	gn = 0;
	Field* compareField = 0;
	INT64 low = 0;
	
#line 1632 "QueryParser.cpp"
	
	match(GREATER);
	match(O_PAREN);
	field = LT(1);
	match(TERM);
	low=number();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 742 "indrilang.g"
		
		compareField = new Field(field->getText());
		gn = new FieldGreaterNode( compareField, low );
		_nodes.push_back( compareField );
		_nodes.push_back( gn );
		
#line 1648 "QueryParser.cpp"
	}
	return gn ;
}

 FieldBetweenNode*  QueryParser::betweenNode() {
#line 761 "indrilang.g"
	 FieldBetweenNode* bn ;
#line 1656 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 761 "indrilang.g"
	
	bn = 0;
	Field* compareField = 0;
	INT64 low = 0;
	INT64 high = 0;
	
#line 1665 "QueryParser.cpp"
	
	match(BETWEEN);
	match(O_PAREN);
	field = LT(1);
	match(TERM);
	low=number();
	high=number();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 767 "indrilang.g"
		
		compareField = new Field(field->getText());
		bn = new FieldBetweenNode( compareField, low, high );
		_nodes.push_back( compareField );
		_nodes.push_back( bn );
		
#line 1682 "QueryParser.cpp"
	}
	return bn ;
}

 FieldEqualsNode*  QueryParser::equalsNode() {
#line 774 "indrilang.g"
	 FieldEqualsNode* en ;
#line 1690 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 774 "indrilang.g"
	
	en = 0;
	Field* compareField = 0;
	INT64 eq = 0;
	
#line 1698 "QueryParser.cpp"
	
	match(EQUALS);
	match(O_PAREN);
	field = LT(1);
	match(TERM);
	eq=number();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 779 "indrilang.g"
		
		compareField = new Field(field->getText());
		en = new FieldEqualsNode( compareField, eq );
		_nodes.push_back( compareField );
		_nodes.push_back( en );
		
#line 1714 "QueryParser.cpp"
	}
	return en ;
}

 IndexTerm*  QueryParser::rawText() {
#line 682 "indrilang.g"
	 IndexTerm* t ;
#line 1722 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  et = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  qet = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 682 "indrilang.g"
	
	t = 0;
	
#line 1732 "QueryParser.cpp"
	
	switch ( LA(1)) {
	case TERM:
	{
		id = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 685 "indrilang.g"
			
			t = new indri::lang::IndexTerm(id->getText());
			_nodes.push_back(t);
			
#line 1745 "QueryParser.cpp"
		}
		break;
	}
	case NUMBER:
	{
		n = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 689 "indrilang.g"
			
			t = new indri::lang::IndexTerm(n->getText());
			_nodes.push_back(t);
			
#line 1759 "QueryParser.cpp"
		}
		break;
	}
	case FLOAT:
	{
		f = LT(1);
		match(FLOAT);
		if ( inputState->guessing==0 ) {
#line 693 "indrilang.g"
			
			t = new indri::lang::IndexTerm(f->getText());
			_nodes.push_back(t);
			
#line 1773 "QueryParser.cpp"
		}
		break;
	}
	case DBL_QUOTE:
	{
		match(DBL_QUOTE);
		t=rawText();
		match(DBL_QUOTE);
		if ( inputState->guessing==0 ) {
#line 697 "indrilang.g"
			
			// if a text term appears in quotes, consider it stemmed
			t->setStemmed(true);
			
#line 1788 "QueryParser.cpp"
		}
		break;
	}
	case ENCODED_TERM:
	{
		et = LT(1);
		match(ENCODED_TERM);
		if ( inputState->guessing==0 ) {
#line 701 "indrilang.g"
			
			std::string decodedString; 
			base64_decode_string(decodedString, et->getText());
			t = new indri::lang::IndexTerm( decodedString );
			_nodes.push_back(t);
			
#line 1804 "QueryParser.cpp"
		}
		break;
	}
	case ENCODED_QUOTED_TERM:
	{
		qet = LT(1);
		match(ENCODED_QUOTED_TERM);
		if ( inputState->guessing==0 ) {
#line 707 "indrilang.g"
			
			std::string decodedString; 
			base64_decode_string(decodedString, et->getText());
			t = new indri::lang::IndexTerm( decodedString );
			t->setStemmed(true);
			_nodes.push_back(t);
			
#line 1821 "QueryParser.cpp"
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return t ;
}

 Field*  QueryParser::field_restriction() {
#line 588 "indrilang.g"
	 Field* extent ;
#line 1836 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  fieldName = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	match(O_SQUARE);
	fieldName = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 590 "indrilang.g"
		
		extent = new Field( fieldName->getText() );
		_nodes.push_back( extent );
		
#line 1848 "QueryParser.cpp"
	}
	match(C_SQUARE);
	return extent ;
}

 UINT64  QueryParser::date() {
#line 648 "indrilang.g"
	 UINT64 d ;
#line 1857 "QueryParser.cpp"
	
	bool synPredMatched215 = false;
	if (((LA(1) == NUMBER))) {
		int _m215 = mark();
		synPredMatched215 = true;
		inputState->guessing++;
		try {
			{
			match(NUMBER);
			match(SLASH);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched215 = false;
		}
		rewind(_m215);
		inputState->guessing--;
	}
	if ( synPredMatched215 ) {
		d=slashDate();
	}
	else {
		bool synPredMatched217 = false;
		if (((LA(1) == NUMBER))) {
			int _m217 = mark();
			synPredMatched217 = true;
			inputState->guessing++;
			try {
				{
				match(NUMBER);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched217 = false;
			}
			rewind(_m217);
			inputState->guessing--;
		}
		if ( synPredMatched217 ) {
			d=spaceDate();
		}
		else if ((LA(1) == TERM)) {
			d=dashDate();
		}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return d ;
}

 UINT64  QueryParser::slashDate() {
#line 667 "indrilang.g"
	 UINT64 d ;
#line 1912 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  month = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  day = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  year = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 667 "indrilang.g"
	
	d = 0;
	
#line 1920 "QueryParser.cpp"
	
	month = LT(1);
	match(NUMBER);
	match(SLASH);
	day = LT(1);
	match(TERM);
	match(SLASH);
	year = LT(1);
	match(NUMBER);
	if ( inputState->guessing==0 ) {
#line 670 "indrilang.g"
		
		d = DateParse::convertDate( year->getText(), month->getText(), day->getText() ); 
		
#line 1935 "QueryParser.cpp"
	}
	return d ;
}

 UINT64  QueryParser::spaceDate() {
#line 674 "indrilang.g"
	 UINT64 d ;
#line 1943 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  day = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  month = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  year = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 674 "indrilang.g"
	
	d = 0;
	
#line 1951 "QueryParser.cpp"
	
	day = LT(1);
	match(NUMBER);
	month = LT(1);
	match(TERM);
	year = LT(1);
	match(NUMBER);
	if ( inputState->guessing==0 ) {
#line 677 "indrilang.g"
		
		d = DateParse::convertDate( year->getText(), month->getText(), day->getText() );
		
#line 1964 "QueryParser.cpp"
	}
	return d ;
}

 UINT64  QueryParser::dashDate() {
#line 653 "indrilang.g"
	 UINT64 d ;
#line 1972 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  dmy = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 653 "indrilang.g"
	
	d = 0;
	
#line 1978 "QueryParser.cpp"
	
	dmy = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 656 "indrilang.g"
		
		const std::string& text = dmy->getText();
		int firstDash = text.find('-');
		int secondDash = text.find('-', firstDash+1);
		std::string day = text.substr( 0, firstDash ); 
		std::string month = text.substr( firstDash+1, secondDash-firstDash-1 );
		std::string year = text.substr( secondDash );
		
		d = DateParse::convertDate( year, month, day ); 
		
#line 1994 "QueryParser.cpp"
	}
	return d ;
}

 INT64  QueryParser::number() {
#line 727 "indrilang.g"
	 INT64 v ;
#line 2002 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  nn = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 727 "indrilang.g"
	
	v = 0;
	
#line 2009 "QueryParser.cpp"
	
	switch ( LA(1)) {
	case NUMBER:
	{
		n = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 730 "indrilang.g"
			
			v = string_to_i64(n->getText());
			
#line 2021 "QueryParser.cpp"
		}
		break;
	}
	case NEGATIVE_NUMBER:
	{
		nn = LT(1);
		match(NEGATIVE_NUMBER);
		if ( inputState->guessing==0 ) {
#line 733 "indrilang.g"
			
			v = string_to_i64(nn->getText());
			
#line 2034 "QueryParser.cpp"
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return v ;
}

void QueryParser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& )
{
}
const char* QueryParser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"\"#sum\"",
	"\"#wsum\"",
	"\"#wand\"",
	"\"#od\"",
	"\"#or\"",
	"\"#not\"",
	"\"#uw\"",
	"\"#combine\"",
	"\"#weight\"",
	"\"#max\"",
	"\"#filreq\"",
	"\"#filrej\"",
	"\"#any\"",
	"\"#band\"",
	"\"#syn\"",
	"\"#prior\"",
	"\"#date:after\"",
	"\"#date:before\"",
	"\"#date:between\"",
	"\"#less\"",
	"\"#greater\"",
	"\"#between\"",
	"\"#equals\"",
	"NUMBER",
	"NEGATIVE_NUMBER",
	"FLOAT",
	"STAR",
	"O_PAREN",
	"C_PAREN",
	"O_ANGLE",
	"C_ANGLE",
	"O_SQUARE",
	"C_SQUARE",
	"O_BRACE",
	"C_BRACE",
	"DBL_QUOTE",
	"QUOTE",
	"DOT",
	"COMMA",
	"DASH",
	"COLON",
	"TAB",
	"CR",
	"LF",
	"SPACE",
	"HIGH_CHAR",
	"DIGIT",
	"ASCII_LETTER",
	"SAFE_LETTER",
	"SAFE_CHAR",
	"BASESIXFOUR_CHAR",
	"TEXT_TERM",
	"TERM",
	"ENCODED_QUOTED_TERM",
	"ENCODED_TERM",
	"OPERATOR",
	"JUNK",
	"\"passage\"",
	"SLASH",
	0
};

const unsigned long QueryParser::_tokenSet_0_data_[] = { 805306336UL, 251658402UL, 0UL, 0UL };
// "#wsum" "#wand" "#od" "#or" "#not" "#uw" "#combine" "#weight" "#max" 
// "#filreq" "#filrej" "#any" "#band" "#syn" "#prior" "#date:after" "#date:before" 
// "#date:between" "#less" "#greater" "#between" "#equals" NUMBER FLOAT 
// O_ANGLE O_BRACE DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long QueryParser::_tokenSet_1_data_[] = { 804717696UL, 251658402UL, 0UL, 0UL };
// "#od" "#uw" "#any" "#band" "#syn" "#date:after" "#date:before" "#date:between" 
// "#less" "#greater" "#between" "#equals" NUMBER FLOAT O_ANGLE O_BRACE 
// DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_1(_tokenSet_1_data_,4);


ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
