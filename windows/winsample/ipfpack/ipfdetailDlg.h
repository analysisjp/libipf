#pragma once
#include "afxwin.h"


// ipfdetailDlg �_�C�A���O

class ipfdetailDlg : public CDialog
{
	DECLARE_DYNAMIC(ipfdetailDlg)

public:
	ipfdetailDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~ipfdetailDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_IPFDETAIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_complv_combo;
	int m_revision;
	int m_baserevision;
	int m_complv;
	afx_msg void OnCbnSelchangeIpfdetailComplv();
	afx_msg void OnBnClickedOk();
};
