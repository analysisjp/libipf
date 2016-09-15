/*
 *  unipfDlg
 *
 *  Copyright (C) 2016 analysisjp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#pragma once
#include "libipf.hpp"
#include "afxwin.h"


class unipfDlg : public CDialog
{
// �R���X�g���N�V����
public:
	unipfDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_UNIPF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;
	ipf_file	m_Ipffile;
	ipf_table	m_IpfInfo;

	bool OpenIPFFile(std::ifstream &fin);
	void PutVerbose(LPCTSTR lpszFormat, ...);
	void ClearVerbose();


	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUnipfFileBtn();
	CString m_ipffname;
	CString m_ipfdirname;
	afx_msg void OnBnClickedUnipfDirBtn();
	afx_msg void OnBnClickedUnipfInfoBtn();
	afx_msg void OnBnClickedUnipfUncompBtn();
	CEdit m_verbosemes;
};
