/*
 *  unipfDlg
 *
 *  Copyright (C) 2016 analysisjp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "stdafx.h"
#include "unipf.h"
#include "libipf.hpp"
#include "unipfDlg.h"
#include "CDirDialog.h"
#include "CDirectory.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

#include <stdarg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



// �t�H���_�I���_�C�A���O��\�����鋤�ʊ֐�
static bool DoModalFolderDlg(CString &rhdir,LPCTSTR ftype)
{
	CString tmpdir;
	CDirDialog dlg(TRUE, "", "", OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, ftype);

	dlg.m_bSelectableFolder=TRUE;
	if(! rhdir.IsEmpty()){
		dlg.m_ofn.lpstrInitialDir = rhdir;
	}
	if(dlg.DoModal() != IDOK) return false;

	CDirectory tmppath = CDirectory(dlg.GetPathName());

	if(tmppath.IsPathStrict() == FALSE){
		rhdir = tmppath.GetFilePath();
	}else{
		rhdir = tmppath;
	}

	if(rhdir.IsEmpty()){
		return false;
	}
	return true;
}




// unipfDlg �_�C�A���O

unipfDlg::unipfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(unipfDlg::IDD, pParent)
	, m_ipffname(_T(""))
	, m_ipfdirname(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void unipfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UNIPF_FILENAME, m_ipffname);
	DDX_Text(pDX, IDC_UNIPF_DIRNAME, m_ipfdirname);
	DDX_Control(pDX, IDC_UNIPF_VERBOSE, m_verbosemes);
}

BEGIN_MESSAGE_MAP(unipfDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_UNIPF_FILE_BTN, &unipfDlg::OnBnClickedUnipfFileBtn)
	ON_BN_CLICKED(IDC_UNIPF_DIR_BTN, &unipfDlg::OnBnClickedUnipfDirBtn)
	ON_BN_CLICKED(IDC_UNIPF_INFO_BTN, &unipfDlg::OnBnClickedUnipfInfoBtn)
	ON_BN_CLICKED(IDC_UNIPF_UNCOMP_BTN, &unipfDlg::OnBnClickedUnipfUncompBtn)
END_MESSAGE_MAP()


// unipfDlg ���b�Z�[�W �n���h��

BOOL unipfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// CEdit�̌��E���ł��������
	m_verbosemes.SetLimitText(0);	

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void unipfDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void unipfDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR unipfDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// �t�@�C���I���{�^��
void unipfDlg::OnBnClickedUnipfFileBtn()
{
	CFileDialog dlg(TRUE, _T("ipf"), _T("*.ipf"), OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_ENABLESIZING,
		_T("IPF�t�@�C��(*.ipf)|*.ipf|���ׂẴt�@�C��(*.*)|*.*|"), NULL);

	dlg.m_ofn.lpstrInitialDir = m_ipffname;

	if(dlg.DoModal() == IDCANCEL){
		return;
	}
	CString fname = dlg.GetPathName();
	if(fname.IsEmpty()){
		return;
	}
	UpdateData(TRUE);
	m_ipffname = fname;
	UpdateData(FALSE);
}

// �f�B���N�g���I��
void unipfDlg::OnBnClickedUnipfDirBtn()
{
	CString dirname=m_ipfdirname;

	if(! DoModalFolderDlg(dirname,"all (*.*)|*.*||")) return; 

	UpdateData(TRUE);
	m_ipfdirname = dirname;
	UpdateData(FALSE);
}

void unipfDlg::PutVerbose(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[1200];

	nBuf = _vsnprintf(szBuffer, 1024, lpszFormat, args);

	int xlen = m_verbosemes.GetWindowTextLength();
	m_verbosemes.SetFocus();
	m_verbosemes.SetSel(xlen, xlen);
	m_verbosemes.ReplaceSel(szBuffer);

	va_end(args);
}


void unipfDlg::ClearVerbose()
{
	m_verbosemes.SetFocus();
	m_verbosemes.SetSel(0, -1);
	m_verbosemes.Clear();
}


bool unipfDlg::OpenIPFFile(ifstream &fin)
{
	int status;
	m_IpfInfo.clear();

	PutVerbose(_T("read IPF file %s...\r\n"),m_ipffname);

	fin.open(m_ipffname,ios::in | ios::binary);
	if(! fin.is_open()){
		PutVerbose(_T("ERROR: open file miss!! %s \r\n"),m_ipffname);
		return false;
	}
	status = libipf_read_header((LPCTSTR)m_ipffname,fin,m_Ipffile,m_IpfInfo);
	if(status != IPF_OK){
		PutVerbose(_T("ERROR: read header miss!! (%d)\r\n"),status);
		return false;
	}
	PutVerbose(_T("IPF file in file %lu \r\n\r\n"),m_IpfInfo.size());
	return true;
}


// ���̕\���{�^��
void unipfDlg::OnBnClickedUnipfInfoBtn()
{
	ifstream fin;
	ClearVerbose();

	if(m_ipffname.IsEmpty()){
		AfxMessageBox("IPF�t�@�C�������w�肵�Ă�������");
		return;
	}

	if(! OpenIPFFile(fin)) return;

	std::size_t i;
	
	PutVerbose("IPF �t�@�C�� %s\r\n"				,m_Ipffile.getFileName().c_str());
	PutVerbose("IPF �w�b�_���  ---------------------------------\r\n");
	PutVerbose("IPF���t�@�C���� %d\r\n"				,m_Ipffile.getFileCount());
	PutVerbose("�e�[�u���I�t�Z�b�g %08X\r\n"		,m_Ipffile.getFileTableOffset());
	PutVerbose("�s�� %08X\r\n"						,m_Ipffile.getUnknown());
	PutVerbose("�A�[�J�C�u�w�b�_�I�t�Z�b�g %08X\r\n",m_Ipffile.getFileFooterOffset());
	PutVerbose("�t�H�[�}�b�g %08X\r\n"				,m_Ipffile.getFormat());
	PutVerbose("�x�[�X���r�W���� %08X\r\n"			,m_Ipffile.getBaseRevision());
	PutVerbose("���r�W���� %08X\r\n"				,m_Ipffile.getRevision());
	PutVerbose("---------------------------------\r\n");

	PutVerbose("IPF �t�@�C���e�[�u�� ---------------------------------\r\n");
	PutVerbose(" Fnl  Anl      CRC         Comp       Uncomp   offset arcname filename\r\n");
	for(i=0;i<m_IpfInfo.size();i++){
		PutVerbose(_T("%4d %4d %08X %12d %12d %08X %s %s\r\n")
			,m_IpfInfo[i].getFilenameLength()
			,m_IpfInfo[i].getArcnameLength()
			,m_IpfInfo[i].getCRC()
			,m_IpfInfo[i].getCompressLength()
			,m_IpfInfo[i].getUnCompressLength()
			,m_IpfInfo[i].getDataOffset()
			,m_IpfInfo[i].getArchiveName().c_str()
			,m_IpfInfo[i].getFileName().c_str()
		);
	}
	PutVerbose("---------------------------------\n");

}

// �𓀃{�^��
void unipfDlg::OnBnClickedUnipfUncompBtn()
{
	ifstream fin;
	ClearVerbose();

	if(m_ipffname.IsEmpty()){
		AfxMessageBox("IPF�t�@�C�������w�肵�Ă�������");
		return;
	}
	if(m_ipfdirname.IsEmpty()){
		AfxMessageBox("�𓀐�t�H���_���w�肵�Ă�������");
		return;
	}

	if(! OpenIPFFile(fin)) return;

	std::size_t i;
	ipf_data fdata;
	int status;
	int scnt=0,ecnt=0;

	for(i=0;i<m_IpfInfo.size();i++){
		PutVerbose("�t�@�C���𓀒�... %s/%s \r\n",m_IpfInfo[i].getArchiveName().c_str(),m_IpfInfo[i].getFileName().c_str());
		// ��
		status = libipf_uncompress(fin,m_IpfInfo[i],fdata);
		if(status != IPF_OK){
			PutVerbose("ERROR: �t�@�C���̉𓀂Ɏ��s�I (%d)\r\n",status);
			ecnt++;
			continue;
		}
		// Directory�̍쐬�BFilename�Ƀp�X���܂܂�Ă���/�`���Ȃ̂�Windows�̏ꍇ�͂�₱�����������K�v
		CDirectory dirname(m_ipfdirname);
		CDirectory wfname(m_IpfInfo[i].getFileName().c_str());

		dirname.Append(m_IpfInfo[i].getArchiveName().c_str());
		wfname.ReplaceLinuxPathtoWindowsPath();
		dirname.Append(wfname);
		wfname = dirname;
		dirname.RemoveFileSpec();
		// �t�H���_������΍쐬
		if(! dirname.MakeDirectory()){
			PutVerbose("ERROR: �t�H���_�쐬���s \r\n");
			ecnt++;
			continue;
		}
		// �t�@�C����������
		ofstream fout;
		fout.open(wfname, ios::out|ios::binary|ios::trunc);
		if(! fout){
			PutVerbose("ERROR: �t�@�C���쐬���s \r\n");
			ecnt++;
			continue;
		}
		// �W�J��T�C�Y����̏ꍇ������̂ňꉞ�`�F�b�N���Ă��珑��
		if(! fdata.empty()){
			fout.write((char *) &fdata[0], (std::streamsize)fdata.size());
			if(fout.bad()){
				PutVerbose("ERROR: �t�@�C���������ݎ��s \r\n");
				ecnt++;
				fout.close();
				continue;
			}
		}
		fout.close();
		scnt++;
	}
	PutVerbose("�t�@�C���𓀂��I�����܂��� (�g�[�^�� %d/���� %d/�ُ� %d)\r\n",m_IpfInfo.size(),scnt,ecnt);
}
