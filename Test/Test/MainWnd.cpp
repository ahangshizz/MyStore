#include "MainWnd.h"
#include "MySQL.h"

CDuiString MainWnd::GetSkinFolder() {
	/*_T 是Vs封装的一个宏,他的作用是把ASCII的字符转换为Unicode的形式*/
	return _T("");
}
CDuiString MainWnd::GetSkinFile() {
	return _T("MainWnd - 副本.xml");
	//return _T("Cashier.xml");
}
LPCTSTR MainWnd::GetWindowClassName(void) const {
	return _T("MainWnd");
}

//字符转码
string UnicodeToAsc(const CDuiString& str) {
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,0,str,-1,NULL,0,NULL,NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,0,str,-1,pElementText,iTextLen,NULL,NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

CDuiString AscToUnicode(const string& str) {
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t *  pUnicode;
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	CDuiString  rt;
	rt = (wchar_t*)pUnicode;
	delete  pUnicode;

	return  rt;
}

/***********************************************员工操作***************************************************************/
//查询员工信息
void MainWnd::SelectEmployeeInfo() {
	//构造SQL查询语句
	string StrSQL("select * from employee ");
	//找到Combo控件拿到控件内容
	CComboBoxUI* pCombo = (CComboBoxUI*)m_PaintManager.FindControl(_T("COMOB_SELECT"));
	CDuiString StrStyle = pCombo->GetText();
	if (StrStyle == _T("无"))
		StrSQL += ";";
	else if (StrStyle == _T("姓名")) {
		StrSQL += "where name= '";
		CDuiString StrName = ((CEditUI*)m_PaintManager.FindControl(_T("username")))->GetText();
		if (StrName.IsEmpty()) {
			MessageBox(m_hWnd, _T("请输入待查询姓名"), _T("警告"), IDOK);
			return;
		}
		StrSQL += UnicodeToAsc(StrName);
		StrSQL += "';";
	}
	else if (StrStyle == _T("性别")) {
		StrSQL += "where gender='";
		CDuiString StrGender = ((CEditUI*)m_PaintManager.FindControl(_T("gender")))->GetText();
		if (StrGender.IsEmpty()) {
			MessageBox(m_hWnd, _T("请输入查找条件"), _T("警告"), IDOK);
			return;
		}
		StrSQL += UnicodeToAsc(StrGender);
		StrSQL += "';";
	}
	else if (StrStyle == _T("薪资")) {
		StrSQL += "where salary='";
		CDuiString StrSalary = ((CEditUI*)m_PaintManager.FindControl(_T("salary")))->GetText();
		if (StrSalary.IsEmpty()) {
			MessageBox(m_hWnd, _T("请输入查找条件"), _T("警告"), IDOK);
			return;
		}
		StrSQL += UnicodeToAsc(StrSalary);
		StrSQL += "';";
	}
	else if (StrStyle == _T("电话")) {
		StrSQL += "where telphone='";
		CDuiString StrTel = ((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->GetText();
		if (StrTel.IsEmpty()) {
			MessageBox(m_hWnd, _T("请输入查找条件"), _T("警告"), IDOK);
			return;
		}
		StrSQL += UnicodeToAsc(StrTel);
		StrSQL += "';";
	}
	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
	if (vRet.empty())
		return;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListEmployeeInfo"));
	pList->RemoveAll();//清除上次查询的List
	for (size_t i = 0; i < vRet.size(); ++i) {
		vector<string>& StrItem = vRet[i];
		CListTextElementUI* pDate = new CListTextElementUI;
		pDate->SetAttribute(_T("align"), _T("center"));
		pList->Add(pDate);

		pDate->SetText(0, AscToUnicode(StrItem[1]));
		pDate->SetText(1, AscToUnicode(StrItem[2]));
		pDate->SetText(2, AscToUnicode(StrItem[3]));
		pDate->SetText(3, AscToUnicode(StrItem[5]));
		pDate->SetText(4, AscToUnicode(StrItem[6]));
		pDate->SetText(5, AscToUnicode(StrItem[7]));
	}
}


//删除员工信息
void MainWnd::DeleteEmployeeInfo() {
	
	//获取当前选中
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListEmployeeInfo"));

	//方法
	int RowNum=pList->GetCurSel();
	CListTextElementUI*	pRow =(CListTextElementUI*) pList->GetItemAt(RowNum);

	//在数据库中删除员工信息

	//构造SQL删除命令
	string StrSQL("delete from employee where name='");
	string StrId=UnicodeToAsc(pRow->GetText(0));//拿到第一行的
	StrSQL += StrId;
	StrSQL += "';";
	p_mysql->Delete(StrSQL);

	//CDuiString StrName = pRow->GetText(0);//通过行号拿到List中的名字
	//在List控件中删除员工信息
	pList->RemoveAt(RowNum);
}

void MainWnd::InsertEmployeeInfo() {

	//在输入框拿到员工信息
	CDuiString StrName=((CEditUI*)m_PaintManager.FindControl(_T("username")))->GetText();
	CDuiString StrGender = ((CComboBoxUI*)m_PaintManager.FindControl(_T("gender")))->GetText();
	CDuiString StrBirthday= ((CEditUI*)m_PaintManager.FindControl(_T("birthday")))->GetText();
	CDuiString StrPosition = ((CComboBoxUI*)m_PaintManager.FindControl(_T("position")))->GetText();
	CDuiString StrTel = ((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->GetText();
	CDuiString StrSalay = ((CEditUI*)m_PaintManager.FindControl(_T("salary")))->GetText();


	//pList->GetCount();

	//char buf[32] = { 0 };
	//整形转换为字符
	//_itoa(pList->GetCount()+1, buf, 10);
	//构造SQL命令
	string StrSQL("insert into employee(name,gender,birthday,password,position,telphone,salary) values('");
	//StrSQL += buf;
	//StrSQL += "','";
	StrSQL += UnicodeToAsc(StrName);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrGender);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrBirthday);
	StrSQL += "','123456','";
	StrSQL += UnicodeToAsc(StrPosition);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrTel);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrSalay);
	StrSQL += "');";
	//响应SQL命令
	p_mysql->Insert(StrSQL);
	
	
	/*向List控件中添加该员工信息
	CListTextElementUI* pInfo = new CListTextElementUI;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListEmployeeInfo"));
	pList->Add(pInfo);
	pInfo->SetText(0, StrName);
	pInfo->SetText(1, StrGender);
	pInfo->SetText(2, StrBirthday);
	pInfo->SetText(3, StrPosition);
	pInfo->SetText(4, StrTel);
	pInfo->SetText(5, StrSalay);*/
	
	//可以通过重新查询在List控件中显示员工信息
	SelectEmployeeInfo();

	//将编辑框清空
	((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("salary")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("birthday")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("username")))->SetText(_T(""));
}

void MainWnd::UpDateEmployeeInfo() {
	string StrSQL("update employee set ");
	//默认根据姓名更改员工信息
	//默认只能更改权限/电话/薪资
	CDuiString StrName = ((CEditUI*)m_PaintManager.FindControl(_T("username")))->GetText();
	CDuiString StrPosition = ((CComboBoxUI*)m_PaintManager.FindControl(_T("position")))->GetText();
	CDuiString StrTel = ((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->GetText();
	CDuiString StrSalay = ((CEditUI*)m_PaintManager.FindControl(_T("salary")))->GetText();

	//构造SQL语句
	if (!StrPosition.IsEmpty()) {
		StrSQL += "position='";
		StrSQL += UnicodeToAsc(StrPosition);
		StrSQL += "'";
	}
	if (!StrTel.IsEmpty()&&StrTel!=_T("电话")) {
		if (!StrPosition.IsEmpty()) {
			StrSQL += ",telphone='";
		}
		else {
			StrSQL += "telphone='";
		}
		StrSQL += UnicodeToAsc(StrTel);
		StrSQL += "'";
	}
	if (!StrSalay.IsEmpty()&&StrSalay!=_T("薪资")) {
		if (!StrTel.IsEmpty() || !StrPosition.IsEmpty()) {
			StrSQL += ",salary='";
		}
		else {
			StrSQL += "salary='";
		}
		StrSQL += UnicodeToAsc(StrSalay);
		StrSQL += "'";
	}
	StrSQL += " where name='";
	StrSQL += UnicodeToAsc(StrName);
	StrSQL += "';";
	p_mysql->Update(StrSQL);
	SelectEmployeeInfo();

	//将编辑框清空
	((CEditUI*)m_PaintManager.FindControl(_T("telphone")))->SetText(_T("电话"));
	((CEditUI*)m_PaintManager.FindControl(_T("salary")))->SetText(_T("薪资"));
	((CEditUI*)m_PaintManager.FindControl(_T("birthday")))->SetText(_T("生日"));
	((CEditUI*)m_PaintManager.FindControl(_T("username")))->SetText(_T("姓名"));
}

/***************************************************商品操作**************************************************************/

	//查询商品信息
void MainWnd::SelectGoodsInfo() {
	//构造SQL查询语句
	string StrSQL("select * from goods ");
	//找到Combo控件拿到控件内容
	CComboBoxUI* pCombo = (CComboBoxUI*)m_PaintManager.FindControl(_T("COMBO_SORT"));
	CDuiString StrStyle = pCombo->GetText();
	if (StrStyle == _T("全部"))
		StrSQL += ";";
	else if (StrStyle == _T("烟酒")) {
		StrSQL += "where GoodsType= '烟酒';";
	}
	else if (StrStyle == _T("水果")) {
		StrSQL += "where GoodsType= '水果';";
	}
	else if (StrStyle == _T("日用品")) {
		StrSQL += "where GoodsType= '日用品';";
	}
	else if (StrStyle == _T("零食")) {
		StrSQL += "where GoodsType= '零食';";
	}
	else if (StrStyle == _T("饮料")) {
		StrSQL += "where GoodsType= '饮料';";
	}
	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
	if (vRet.empty())
		return;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_GoodsInfo"));
	pList->RemoveAll();//清除上次查询的List
	for (size_t i = 0; i < vRet.size(); ++i) {
		vector<string>& StrItem = vRet[i];
		CListTextElementUI* pDate = new CListTextElementUI;
		pDate->SetAttribute(_T("align"), _T("center"));
		pList->Add(pDate);

		pDate->SetText(0, AscToUnicode(StrItem[0]));
		pDate->SetText(1, AscToUnicode(StrItem[1]));
		pDate->SetText(2, AscToUnicode(StrItem[2]));
		pDate->SetText(3, AscToUnicode(StrItem[3]));
		pDate->SetText(4, AscToUnicode(StrItem[4]));
		pDate->SetText(5, AscToUnicode(StrItem[5]));
		pDate->SetText(6, AscToUnicode(StrItem[6]));
		pDate->SetText(7, AscToUnicode(StrItem[7]));
		pDate->SetText(8, AscToUnicode(StrItem[8]));
	}
}
//删除商品信息
void MainWnd::DeleteGoodsInfo() {
	//获取当前选中
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("List_GoodsInfo"));

	//方法
	int RowNum = pList->GetCurSel();
	CListTextElementUI*	pRow = (CListTextElementUI*)pList->GetItemAt(RowNum);

	//在数据库中删除商品信息

	//构造SQL删除命令
	string StrSQL("delete from goods where GoodsId='");
	string StrId = UnicodeToAsc(pRow->GetText(0));//拿到第一列的即商品ID列
	StrSQL += StrId;
	StrSQL += "';";
	p_mysql->Delete(StrSQL);

	//CDuiString StrName = pRow->GetText(0);//通过行号拿到List中的名字
	//在List控件中删除员工信息
	pList->RemoveAt(RowNum);
}
//插入商品信息
void MainWnd::InsertGoodsInfo() {
	//在输入框拿到商品信息
	//CDuiString StrID = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSID")))->GetText();//设置主键自增长不需要提供商品ID
	CDuiString StrName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->GetText();
	CDuiString StrProDate = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRO_DATE")))->GetText();
	CDuiString StrSort = ((CComboBoxUI*)m_PaintManager.FindControl(_T("COMBO_SORT")))->GetText();
	CDuiString StrExpDate = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_EXP_DATE")))->GetText();
	CDuiString StrPrice = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRICE")))->GetText();
	CDuiString StrUnit = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_UNIT")))->GetText();
	CDuiString StrLeft = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->GetText();
	CDuiString StrPolice = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_POLICE")))->GetText();


	//pList->GetCount();

	//char buf[32] = { 0 };
	//整形转换为字符
	//_itoa(pList->GetCount()+1, buf, 10);
	//构造SQL命令
	string StrSQL("insert into goods values(");
	//StrSQL += buf;
	//StrSQL += "','";
 	StrSQL += "NULL";
 	StrSQL += ",'";
	StrSQL += UnicodeToAsc(StrName);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrSort);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrProDate);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrExpDate);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrPrice);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrUnit);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrLeft);
	StrSQL += "','";
	StrSQL += UnicodeToAsc(StrPolice);
	StrSQL += "');";
	//响应SQL命令
	p_mysql->Insert(StrSQL);


	/*向List控件中添加该员工信息
	CListTextElementUI* pInfo = new CListTextElementUI;
	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListEmployeeInfo"));
	pList->Add(pInfo);
	pInfo->SetText(0, StrName);
	pInfo->SetText(1, StrGender);
	pInfo->SetText(2, StrBirthday);
	pInfo->SetText(3, StrPosition);
	pInfo->SetText(4, StrTel);
	pInfo->SetText(5, StrSalay);*/


	//将编辑框清空
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->SetText(_T("商品名称"));
	((CComboBoxUI*)m_PaintManager.FindControl(_T("COMBO_SORT")))->SetText(_T("全部"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRO_DATE")))->SetText(_T("生产日期"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_EXP_DATE")))->SetText(_T("到期日期"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRICE")))->SetText(_T("商品价格"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_UNIT")))->SetText(_T("单位"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->SetText(_T("库存"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_POLICE")))->SetText(_T("报警值"));
	
	
	//可以通过重新查询在List控件中显示员工信息
	SelectGoodsInfo();
}
//更新商品信息
void MainWnd::UpDateGoodsInfo() {
	string StrSQL("update goods set ");
	//默认根据姓名更改员工信息
	//默认只能更改权限/电话/薪资
	CDuiString StrName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->GetText();
	CDuiString StrPrice = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRICE")))->GetText();
	CDuiString StrLeft = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->GetText();
	CDuiString StrPolice = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_POLICE")))->GetText();
	//构造SQL语句
	if (!StrPrice.IsEmpty()&&StrPrice!=_T("商品价格")) {
		StrSQL += "Price='";
		StrSQL += UnicodeToAsc(StrPrice);
		StrSQL += "'";
	}
	if (!StrLeft.IsEmpty()&&StrLeft!=_T("库存")) {
		if (!StrPrice.IsEmpty()) {
			StrSQL += ",Inventory='";
		}
		else {
			StrSQL += "Inventory='";
		}
		StrSQL += UnicodeToAsc(StrLeft);
		StrSQL += "'";
	}
	if (!StrPolice.IsEmpty()&&StrPolice!=_T("报警值")) {
		if (!StrPrice.IsEmpty() || !StrLeft.IsEmpty()) {
			StrSQL += ",AlarmValye='";
		}
		else {
			StrSQL += "AlarmValye='";
		}
		StrSQL += UnicodeToAsc(StrPolice);
		StrSQL += "'";
	}
	StrSQL += " where GoodsName='";
	StrSQL += UnicodeToAsc(StrName);
	StrSQL += "';";
	p_mysql->Update(StrSQL);
	SelectGoodsInfo();

	//将编辑框清空
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->SetText(_T("商品名称"));
	((CComboBoxUI*)m_PaintManager.FindControl(_T("COMBO_SORT")))->SetText(_T("全部"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRO_DATE")))->SetText(_T("生产日期"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_EXP_DATE")))->SetText(_T("到期日期"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_PRICE")))->SetText(_T("商品价格"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_UNIT")))->SetText(_T("单位"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->SetText(_T("库存"));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_POLICE")))->SetText(_T("报警值"));
}


/***************************************************消息响应************************************************************/
void MainWnd::Notify(TNotifyUI& msg) {
	//获取控件名字
	CDuiString StrName = msg.pSender->GetName();
	//根据控件类型进行筛选
	if (msg.sType == _T("click")) {          //获取点击消息
		if (StrName == _T("BTN_CLOSE")) {	 //点击界面的空间名称
			Close();
		}
		else if (StrName == _T("BTN_MIN")) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE,0);
		}
		else if (StrName == _T("BTN_SELECT")) {
			SelectEmployeeInfo();
		}
		else if (StrName == _T("BTN_DELETE")) {
			DeleteEmployeeInfo();
		}
		else if (StrName == _T("BTN_UPDATE")) {
			UpDateEmployeeInfo();
		}
		else if (StrName == _T("BTN_INSERT")) {
			InsertEmployeeInfo();
		}
		else if (StrName == _T("BTN_RECORD")) {
			MessageBox(NULL, _T("record"), _T("cashier"), IDOK);
		}
		else if (StrName == _T("BTN_GOODS_SELECT")) {
			SelectGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_DELETE")) {
			DeleteGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_INSERT")) {
			InsertGoodsInfo();
		}
		else if (StrName == _T("BTN_GOODS_UPDATE")) {
			UpDateGoodsInfo();
		}
	}
	else if (msg.sType == _T("selectchanged")) {	//捕获
		//寻找控件
		CTabLayoutUI* pTab =(CTabLayoutUI*)m_PaintManager.FindControl(_T("tablayout"));
		if (StrName == _T("OPTION_EMPLOYEE")) {
			pTab->SelectItem(0);
		}
		else if (StrName == _T("OPTION_GOODS"))
			pTab->SelectItem(1);
		else
			pTab->SelectItem(2);
	}
}