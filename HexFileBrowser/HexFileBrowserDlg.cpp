﻿
// HexFileBrowserDlg.cpp: 实现文件
// 16进制文件浏览器

#include "pch.h"
#include "framework.h"
#include "HexFileBrowser.h"
#include "HexFileBrowserDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHexFileBrowserDlg 对话框



CHexFileBrowserDlg::CHexFileBrowserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HEXFILEBROWSER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHexFileBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_FileList);
	DDX_Control(pDX, IDC_TREE_FILE, m_FilePathTree);
	DDX_Control(pDX, IDC_PROGRESS2, m_ProgressLoad);
}

BEGIN_MESSAGE_MAP(CHexFileBrowserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILEOPEN, &CHexFileBrowserDlg::OnFileopen)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_FILE, &CHexFileBrowserDlg::OnRclickTreeFile)
	ON_COMMAND(ID_OPENSELECT, &CHexFileBrowserDlg::OnOpenSelectFile)
	ON_COMMAND(ID_DELSELECT, &CHexFileBrowserDlg::OnDelSelectFile)
END_MESSAGE_MAP()


// CHexFileBrowserDlg 消息处理程序

BOOL CHexFileBrowserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 初始化树控件(添加一个根结点)
	m_hRoot = m_FilePathTree.InsertItem(L"最近打开的文件", TVI_ROOT, TVI_LAST);

	InitFileListTitle();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHexFileBrowserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHexFileBrowserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHexFileBrowserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID CHexFileBrowserDlg::InitFileListTitle()
{
	// 设置列表风格
	m_FileList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 第0列是显示地址的
	m_FileList.InsertColumn(0, L" ", LVCFMT_CENTER, 80);
	// 16进制区域设置
	m_FileList.InsertColumn(1, L"0", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(2, L"1", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(3, L"2", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(4, L"3", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(5, L"4", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(6, L"5", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(7, L"6", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(8, L"7", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(9, L"8", LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(10,L"9",LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(11,L"A",LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(12,L"B",LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(13,L"C",LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(14,L"D",LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(15,L"E",LVCFMT_CENTER, 30);
	m_FileList.InsertColumn(16,L"F",LVCFMT_CENTER, 30);
	// 
	m_FileList.InsertColumn(17, L"|", 0, 15);
	// ASCII区设置
	m_FileList.InsertColumn(18, L"0", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(19, L"1", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(20, L"2", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(21, L"3", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(22, L"4", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(23, L"5", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(24, L"6", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(25, L"7", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(26, L"8", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(27, L"9", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(28, L"A", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(29, L"B", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(30, L"C", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(31, L"D", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(32, L"E", LVCFMT_CENTER, 20);
	m_FileList.InsertColumn(33, L"F", LVCFMT_CENTER, 20);

	return VOID();
}



void CHexFileBrowserDlg::OnFileopen()
{
	// 快速选择文件获取其完整路径(根据指定的正则)
	CFileDialog objFileDlg(TRUE, NULL, NULL, NULL, L"文本文件|*.txt|可执行文件|*.exe|DLL文件|*.dll|SYS文件|*.sys|所有文件|*.*||", this);
	objFileDlg.DoModal(); // 此处会弹出一个通用对话框(类似于文件搜索的对话框)
	// 点选指定文件后获取到他的完整文件路径+文件名
	CString csPath = objFileDlg.GetPathName();
	// 显示到树控件上
	m_FilePathTree.InsertItem(csPath, m_hRoot, TVI_LAST);
	// 加载文件
	InitFileListData(csPath.GetBuffer(0));
}


void CHexFileBrowserDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR szPath[MAX_PATH] = { 0 };
	UINT uiRes = DragQueryFile(hDropInfo, -1, NULL, 0); // 判断拖拽的文件是否存在
	if (uiRes <= 0)
	{
		return;
	}
	uiRes = DragQueryFile(hDropInfo, uiRes-1, szPath, MAX_PATH); // 获取拖拽的文件的路径与名字
	// 如果确实拖拽进来了那么将其添加到树控件上
	if (szPath != NULL)
	{
		m_FilePathTree.InsertItem(szPath, m_hRoot, TVI_LAST);
	}
	// 加载文件
	InitFileListData(szPath);
	CDialogEx::OnDropFiles(hDropInfo);
}

VOID CHexFileBrowserDlg::InitFileListData(TCHAR* szPath)
{
	// 清空缓冲区准备读取文件
	if (m_szBuffer != nullptr)
	{
		delete m_szBuffer;
	}
	// 清空列表区准备显示文件
	m_FileList.DeleteAllItems();
	InitFileListTitle();// 重新生成一遍
	// 开始读取文件到缓冲区
	// 使用CFile类创建对象
	CFile objFile;
	// 以读写模式打开文件
	objFile.Open(szPath, CFile::modeReadWrite);
	// 获取文件长度
	ULONGLONG ullLength = objFile.GetLength();
	// 根据文件长度申请内存空间并读取文件内容到指定缓冲区
	m_szBuffer = new TCHAR[ullLength + 1];
	memset(m_szBuffer, 0, ullLength + 1);
	// 实际读取长度
	UINT uiReadSize = 0;
	uiReadSize = objFile.Read(m_szBuffer, ullLength);
	
	// 以16进制形式读取显示文件到列表处(每16字节就进位)
	DWORD dwLines = uiReadSize / 16 + 1; // 获取行数
	DWORD dwLineCount = 0; // 行计数
	DWORD dwIndex = 0; // 行计数
	CString csValue; // 逐行读取的缓冲区

	m_ProgressLoad.SetRange32(0, dwLines); // 以行数来计算进度
	// 开始读取并显示到控件上
	for (size_t i = 0; i < dwLines; i++) // 外层循环以行计算
	{
		csValue.Format(L"%08X", dwLineCount); // 首先将地址逐行格式化存到csValue中
		m_FileList.InsertItem(dwIndex, csValue); // 设置到当前行的第0列上
		// 开始第一遍读取(显示16进制码)
		for (size_t j = 0; j < 16; j++) // 外层循环以列计算
		{
			UCHAR ucCode = m_szBuffer[i + j]; // 逐字节读入到ucCode
			csValue.Format(L"%02X", ucCode); // 一个字节以2个16进制数格式化输入到csValue中
			m_FileList.SetItemText(dwIndex, j + 1, csValue); // 再设置到当前行的第j+1列上
		}
		m_FileList.SetItemText(dwIndex, 17, L"|"); // 16进制显示结束插入分隔符
		// 开始第二遍读取(显示ASCII码)
		for (size_t j = 0; j < 16; j++)
		{
			UCHAR ucCode = m_szBuffer[i + j]; // 逐字节读入到ucCode
			if (ucCode == 00)
			{
				csValue.Format(L"."); // 如果碰到不可显示字符以.代替
			}
			else
			{
				csValue.Format(L"%c", ucCode);
			}
			m_FileList.SetItemText(dwIndex, j + 18, csValue); // 设置到当前行的第j+18列上(因为ascii区域从第18列开始)
		}
		dwLineCount += 0x10;// 每行的地址数都是16个
		dwIndex++; // 行数++
		m_ProgressLoad.SetPos(i);
	}
	// 加载完毕后将进度条清零
	m_ProgressLoad.SetPos(0);
	// 关闭文件对象
	objFile.Close();

	delete m_szBuffer;
	m_szBuffer = nullptr;
}


void CHexFileBrowserDlg::OnRclickTreeFile(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POINT pt = { 0 };
	GetCursorPos(&pt); // 获取点击位置,稍后弹出时需要用到
	// 获取主菜单IDC_LIST2
	HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDR_MENUTREE));
	// 获取子菜单
	HMENU hSubMenu = GetSubMenu(hMenu, NULL);
	// 弹出菜单
	TrackPopupMenu(hSubMenu, TPM_CENTERALIGN, pt.x, pt.y, 0, m_hWnd, NULL);

	*pResult = 0;
}


void CHexFileBrowserDlg::OnOpenSelectFile()
{
	// 注意此处需要左键双击才算做选中
	HTREEITEM hCurr = m_FilePathTree.GetSelectedItem(); // 获取选中结点
	CString csItem = m_FilePathTree.GetItemText(hCurr); // 获取选中结点的内容
	// 在编辑器中打开所选文件
	InitFileListData(csItem.GetBuffer(0));
}


void CHexFileBrowserDlg::OnDelSelectFile()
{
	// 注意此处需要左键双击才算做选中
	HTREEITEM hCurr = m_FilePathTree.GetSelectedItem(); // 获取选中结点
	// 删除选中结点
	m_FilePathTree.DeleteItem(hCurr);
}