// TopToolDlg.h : ͷ�ļ�
//

#pragma once
#include "CJHyperLink.h"

// CTopToolDlg �Ի���
class CTopToolDlg : public CDialog
{
// ����
public:
	CTopToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TOPTOOL_DIALOG };
	CCJHyperLink	m_staticEmail;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
