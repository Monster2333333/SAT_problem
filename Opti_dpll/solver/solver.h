/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf����Ż�ģ�����ͷ�ļ�		                          
/**********************************************************/

#pragma once
#include"./tool_functions.h"
#include"../cnfparser/cnfparser.h"
#include"../../display/debug.h"


bool dpllOpti(struct Formula*, int*, int*);


/**
@brief: ������ѡ�����ָ�ֵ�����д���
@param formula: ָ��ʽ��ָ��
@param res: ����������ָ�ֵ������
@param counter: ������
@param selected_data: ��ѡ���������
*/
void applySelectedData(struct Formula* formula, int* res, int* counter, int selected_data)
{
	res[abs(selected_data)] = selected_data;	//��ֵ
	removeClauseHasLiteralOpti(formula, selected_data, counter);
	removeLiteralFromFormulaOpti(formula, -selected_data, counter);
}


/**
@brief: dpll��֧����
@param formula: ָ��ʽ��ָ��
@param res: ��ű�����ֵ������
@param counter: ������
@param data: ���ݷ�֧����ѡȡ������
@return: ����ǰ��֧��ʽ�������򷵻�true
*/
bool branchOpti(struct Formula* formula, int* res, int* counter, int data)
{
	applySelectedData(formula, res, counter, data);
	return dpllOpti(formula, res, counter);
}


/**
@brief: dpll�㷨������
@param formula: ָ��ʽ��ָ��
@param res: �������ָ�ֵ������
@param counter: ������
@return: ����ʽ�������򷵻�true
*/
bool dpllOpti(struct Formula* formula, int* res, int* counter)
{
	int selected_data = 0;

	while ((selected_data = selectDataFromUnitClauseOpti(formula, counter))) {				//ѡȡ���Ӿ���е��Ӿ䴫��
		applySelectedData(formula, res, counter, selected_data);
		if (formula->num_clause == 0) return true;		//��ʽȫ��˵����ʽ������
		else if (hasVoidClause(formula)) return false;//�п��Ӿ�˵����ʽ��������
	}
	if (formula->num_clause == 0) return true;
	struct Formula* formula_copy = copyFormula(formula);		//���ƹ�ʽ
	int* counter_copy = copyCounter(counter);
	selected_data = selectData(formula, counter);
	if (branchOpti(formula_copy, res, counter_copy, selected_data)) return true;			//��֧1
	else {		//��֧2
		destoryFormula(formula_copy);
		free(counter_copy);
		return branchOpti(formula, res, counter, -selected_data);
	}
}


/**
@brief: ����dpll�㷨�Ľӿ�
@param formula: ָ��ʽ��ָ��
@param counter: ������
@return: ��ʾ�����Result�ṹ��
*/
struct Result DPLLOpti(struct Formula* formula, int* counter)
{
	struct Result result;
	result.isSatisfied = false;		//��ʼ�����
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = dpllOpti(formula, result.res, counter);			//�������
	return result;
}