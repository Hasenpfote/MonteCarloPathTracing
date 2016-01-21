#ifndef __LIB_COMMON_H_
#define __LIB_COMMON_H_

#include <ios>
#include <assert.h>

#define _num2str(num)		#num
#define num2str(num)		_num2str(num)				//!< �����񉻃}�N��

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		 { if((p)){ delete   (p); (p) = NULL; } }	//!< �`�F�b�N�t�� delete �}�N��
#endif // !SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if((p)){ delete[] (p); (p) = NULL; } }	//!< �`�F�b�N�t�� delete[] �}�N��
#endif // !SAFE_DELETE_ARRAY

/*!
	@brief ���b�Z�[�W�t�A�T�[�g�}�N��
	@note  �J���}���Z�q�͗D�揇�ʂ��Ⴂ
	@note  �J���}�ŋ�؂����Ō�̒l���Q�Ƃ���邪�A����ȑO�̂��͎̂��s�݂̂����
	@note  (exp) || (_assert(exp), 0)
	@note  	exp == 0 : (0 || 0) �Ŏ��̏����ł���A_assert(exp) ���R�[�������
	@note  	exp == 1 : (1 || 0) �ŏ�������I�����A_assert(exp) �̓R�[������Ȃ�
	@note  ���ۂ̃R�[�h�͈قȂ邪�C���[�W�͉��L�̂悤�Ȋ���
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