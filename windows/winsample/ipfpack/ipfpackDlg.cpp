// ipfpackDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ipfpack.h"
#include "libipf.hpp"
#include "ipfpackDlg.h"
#include "ipfdetailDlg.h"
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
#include <algorithm>
#include <functional>

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



// ipfpackDlg �_�C�A���O

ipfpackDlg::ipfpackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ipfpackDlg::IDD, pParent)
	, m_ipfdirname(_T(""))
	, m_ipffname(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ipfpackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IPFPACK_DIRNAME, m_ipfdirname);
	DDX_Text(pDX, IDC_IPFPACK_FILENAME, m_ipffname);
	DDX_Control(pDX, IDC_IPFPACK_VERBOSE, m_verbosemes);
}

BEGIN_MESSAGE_MAP(ipfpackDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_IPFPACK_DETAIL_BTN, &ipfpackDlg::OnBnClickedIpfpackDetailBtn)
	ON_BN_CLICKED(IDC_IPFPACK_COMP_BTN, &ipfpackDlg::OnBnClickedIpfpackCompBtn)
	ON_BN_CLICKED(IDC_IPFPACK_DIR_BTN, &ipfpackDlg::OnBnClickedIpfpackDirBtn)
	ON_BN_CLICKED(IDC_IPFPACK_FILE_BTN, &ipfpackDlg::OnBnClickedIpfpackFileBtn)
END_MESSAGE_MAP()


// ipfpackDlg ���b�Z�[�W �n���h��

BOOL ipfpackDlg::OnInitDialog()
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

	// TODO: �������������ɒǉ����܂��B
	m_base_rev = 0;
	m_revision = 0;
	m_comp_level = 1;

	// CEdit�̌��E���ł��������
	m_verbosemes.SetLimitText(0);	

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void ipfpackDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void ipfpackDlg::OnPaint()
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
HCURSOR ipfpackDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// �f�B���N�g���I��
void ipfpackDlg::OnBnClickedIpfpackDirBtn()
{
	CString dirname=m_ipfdirname;

	if(! DoModalFolderDlg(dirname,"all (*.*)|*.*||")) return; 

	UpdateData(TRUE);
	m_ipfdirname = dirname;
	UpdateData(FALSE);
}

// �t�@�C���I���{�^��
void ipfpackDlg::OnBnClickedIpfpackFileBtn()
{
	CFileDialog dlg(FALSE, _T("ipf"), _T("*.ipf"), OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_ENABLESIZING,
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

void ipfpackDlg::PutVerbose(LPCTSTR lpszFormat, ...)
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
	TRACE(szBuffer);

	va_end(args);
}

void ipfpackDlg::ClearVerbose()
{
	m_verbosemes.SetFocus();
	m_verbosemes.SetSel(0, -1);
	m_verbosemes.Clear();
}


// �ڍאݒ�{�^��
void ipfpackDlg::OnBnClickedIpfpackDetailBtn()
{
	ipfdetailDlg dlg;

	dlg.m_baserevision = m_base_rev;
	dlg.m_revision = m_revision ;
	dlg.m_complv = m_comp_level;

	if(dlg.DoModal() != IDOK) return;

	m_base_rev = dlg.m_baserevision;
	m_revision = dlg.m_revision;
	m_comp_level = dlg.m_complv;

}

int ipfpackDlg::createFilelist(CString dirname,ipf_table &ipffiles,const CString &arcname,CString prefname)
{
	CString findname = dirname + "\\*.*";
	CFileFind arcfinder;

	BOOL b_arkfind = arcfinder.FindFile(findname);
	if(b_arkfind == FALSE){
		return -1;
	}

	while(b_arkfind){
		b_arkfind = arcfinder.FindNextFile();
		if(arcfinder.IsDots())	continue;

		CString path = arcfinder.GetFilePath();

		if(arcfinder.IsDirectory()){
			// �f�B���N�g��������
			CString newpref = prefname + arcfinder.GetFileName() + "/";
			if(createFilelist(path,ipffiles,arcname,newpref) != 0){
				return -1;
			}
		}else{
			// �t�@�C��������
			ipf_element element;
			CString fname = prefname + arcfinder.GetFileName();

			element.setFullPath((LPCTSTR) path);
			element.setArchiveName((LPCTSTR) arcname);
			element.setFileName((LPCTSTR) fname);
			element.setUnCompressLength((uint32_t) arcfinder.GetLength());

			ipffiles.push_back(element);
		}
	}
	return 0;
}

// IPF�t�@�C�����X�g�͑啶���ɕϊ�������Ŕ�r����K�v�����邽��
bool compare_fname_nocase_upper(const ipf_element& left,const ipf_element& right)
{
	string upl,upr;
	upl.resize(left.getFullPath().size());
	std::transform(
		left.getFullPath().begin(),
		left.getFullPath().end(),
		upl.begin(),
		::toupper
	);
	upr.resize(right.getFullPath().size());
	std::transform(
		right.getFullPath().begin(),
		right.getFullPath().end(),
		upr.begin(),
		::toupper
	);
	return upl < upr;
}

// ���k�{�^��
void ipfpackDlg::OnBnClickedIpfpackCompBtn()
{
	ClearVerbose();

	if(m_ipffname.IsEmpty()){
		AfxMessageBox("IPF�t�@�C�������w�肵�Ă�������");
		return;
	}
	if(m_ipfdirname.IsEmpty()){
		AfxMessageBox("���k���t�H���_���w�肵�Ă�������");
		return;
	}

	// �A�[�J�C�u���X�g���쐬
	CString findname = m_ipfdirname + "\\*.*";
	CFileFind arcfinder;
	ipf_table ipffiles;
	int status;
	int i;

	BOOL b_arkfind = arcfinder.FindFile(findname);

	PutVerbose("�t�H���_�ɂ���t�@�C���̃��X�g���쐬���Ă��܂�... \r\n");

	if(b_arkfind == FALSE){
		PutVerbose("ERROR: ���k���t�H���_���J���̂Ɏ��s \r\n");
		return;
	}

	while(b_arkfind){
		b_arkfind = arcfinder.FindNextFile();
		if(arcfinder.IsDots())	continue;	// .�̎��͂��̂܂�

		if(! arcfinder.IsDirectory()) continue;
		// �f�B���N�g��������
		createFilelist(arcfinder.GetFilePath(),
			ipffiles,
			arcfinder.GetFileName(),
			""
		);
	}

	// �����\�[�g(�啶����������ʂȂ����啶���x�[�X)
	std::sort(ipffiles.begin(),ipffiles.end(),compare_fname_nocase_upper);
	PutVerbose("�t�@�C������ %lu \r\n",ipffiles.size());

	ofstream fout;
	fout.open((LPCTSTR)m_ipffname,ios::out | ios::binary);

	if(! fout.is_open()){
		PutVerbose("ERROR: ipf�t�@�C���̍쐬�Ɏ��s\r\n");
		return;
	}
	for(i=0;i<ipffiles.size();i++){
		ifstream fin;
		ipf_data rdata;

		PutVerbose("�t�@�C�����k��... %s/%s \r\n",ipffiles[i].getArchiveName().c_str(),ipffiles[i].getFileName().c_str());

		fin.open(ipffiles[i].getFullPath().c_str(), ios::in | ios::binary);
		if(! fin.is_open()){
			PutVerbose("ERROR: ���k�Ώۃt�@�C�����J���̂Ɏ��s�@%s \r\n",ipffiles[i].getFullPath().c_str());
			return;
		}
		rdata.resize(ipffiles[i].getUnCompressLength());
		fin.read((char *) &rdata[0],ipffiles[i].getUnCompressLength());
		if(!fin){
			PutVerbose("ERROR: ���k�Ώۃt�@�C���̓ǂݍ��݂Ɏ��s�@%s \r\n",ipffiles[i].getFullPath().c_str());
			return;
		}
		fin.close();

		status = libipf_compress(fout,ipffiles[i],rdata,m_comp_level);
		if(status != IPF_OK){
			PutVerbose("ERROR: ���k�Ώۃt�@�C���̈��k�Ɏ��s�@%d \r\n",status);
			return;
		}
	}
	status = libipf_write_header_info(
		fout,
		ipffiles,
		m_base_rev,
		m_revision
	);
	fout.close();
	if(status != IPF_OK){
		PutVerbose("ERROR: IPF�t�@�C���̃w�b�_�������݂Ɏ��s�@%d \r\n",status);
		return;
	}
	PutVerbose("IPF�t�@�C�����쐬���܂���\r\n");

#if 0
	PutVerbose("IPF �t�@�C���e�[�u�� ---------------------------------\r\n");
	PutVerbose(" Fnl  Anl      CRC         Comp       Uncomp   offset arcname filename\r\n");
	for(i=0;i<ipffiles.size();i++){
		PutVerbose(_T("%4d %4d %08X %12d %12d %08X %s %s\r\n")
			,ipffiles[i].getFilenameLength()
			,ipffiles[i].getArcnameLength()
			,ipffiles[i].getCRC()
			,ipffiles[i].getCompressLength()
			,ipffiles[i].getUnCompressLength()
			,ipffiles[i].getDataOffset()
			,ipffiles[i].getArchiveName().c_str()
			,ipffiles[i].getFileName().c_str()
		);
	}
	PutVerbose("---------------------------------\n");
#endif

}

