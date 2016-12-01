// ipfpackDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"


// ipfpackDlg �_�C�A���O
class ipfpackDlg : public CDialog
{
// �R���X�g���N�V����
public:
	ipfpackDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_IPFPACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	uint32_t m_base_rev;
	uint32_t m_revision;
	int m_comp_level;

	void PutVerbose(LPCTSTR lpszFormat, ...);
	void ClearVerbose();

	int createFilelist(CString dirname,ipf_table &ipffiles,const CString &arcname,CString prefname);

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIpfpackDetailBtn();
	afx_msg void OnBnClickedIpfpackCompBtn();
	CString m_ipfdirname;
	CString m_ipffname;
	afx_msg void OnBnClickedIpfpackDirBtn();
	afx_msg void OnBnClickedIpfpackFileBtn();
	CEdit m_verbosemes;
};
