
// HexFileBrowserDlg.h: 头文件
//

#pragma once


// CHexFileBrowserDlg 对话框
class CHexFileBrowserDlg : public CDialogEx
{
// 构造
public:
	CHexFileBrowserDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEXFILEBROWSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 用于更改对话框标题
	CString m_csCaption;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 显示文件的树的根结点
	HTREEITEM m_hRoot;
	// 16进制显示文件列表控件
	CListCtrl m_FileList;
	// 读取文件的缓冲区
	TCHAR* m_szBuffer = nullptr;
	// 初始化列表控件(设置列表头)
	VOID InitFileListTitle();
	// 打开文件
	afx_msg void OnFileopen();
	// 树控件显示文件
	CTreeCtrl m_FilePathTree;
	// 接受拖拽文件
	afx_msg void OnDropFiles(HDROP hDropInfo);
	// 读取并以16进制形式显示文件
	VOID InitFileListData(TCHAR* szPath);
	// 显示加载进度
	CProgressCtrl m_ProgressLoad;
	// 弹出树控件的右键菜单
	afx_msg void OnRclickTreeFile(NMHDR* pNMHDR, LRESULT* pResult);
	// 打开所选文件
	afx_msg void OnOpenSelectFile();
	// 删除所选文件
	afx_msg void OnDelSelectFile();
};
