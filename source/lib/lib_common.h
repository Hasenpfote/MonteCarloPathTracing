#ifndef __LIB_COMMON_H_
#define __LIB_COMMON_H_

#include <ios>
#include <assert.h>

#define _num2str(num)		#num
#define num2str(num)		_num2str(num)				//!< 文字列化マクロ

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		 { if((p)){ delete   (p); (p) = NULL; } }	//!< チェック付き delete マクロ
#endif // !SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if((p)){ delete[] (p); (p) = NULL; } }	//!< チェック付き delete[] マクロ
#endif // !SAFE_DELETE_ARRAY

/*!
	@brief メッセージ付アサートマクロ
	@note  カンマ演算子は優先順位が低い
	@note  カンマで区切った最後の値が参照されるが、それ以前のものは実行のみされる
	@note  (exp) || (_assert(exp), 0)
	@note  	exp == 0 : (0 || 0) で次の条件である、_assert(exp) がコールされる
	@note  	exp == 1 : (1 || 0) で条件分岐終了し、_assert(exp) はコールされない
	@note  実際のコードは異なるがイメージは下記のような感じ
	@note  if(exp || 0){ nop; } else { _assert(exp); }
*/
#ifndef ASSERT_MSG
#define ASSERT_MSG(exp,msg)	 assert((exp)&&(msg))
#endif // !ASSERT_MSG

/*
	printf("%s\n", __DATE__);
	printf("%s\n", __FILE__);
	printf("%d\n", __LINE__);
	printf("%s\n", __TIME__);
	printf("%s\n", __TIMESTAMP__ );
*/
#endif // !__LIB_COMMON_H_