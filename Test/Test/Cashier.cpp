#include "Cashier.h"




CDuiString CashierWnd::GetSkinFolder() {
	return _T("");
}
CDuiString CashierWnd::GetSkinFile() {
	return _T("Cashier.xml");
}
LPCTSTR CashierWnd::GetWindowClassName(void) const {
	return _T("CashierWnd");
}

  void CashierWnd::SelectGoods() {
 	//从控件找到商品名
 	CDuiString StrGoodName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->GetText();
 	//构造SQL语句
 	string StrSQL("select * from goods where GoodsName='");
 	StrSQL += UnicodeToAsc(StrGoodName);
 	StrSQL += "';";
 	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
 	if (vRet.empty()) {
 		MessageBox(m_hWnd, _T("没有该商品"), _T("警告"), IDOK);
 		return;
 	}


 	//将商品显示
	string str = vRet[0][7];
	CDuiString StrLeft = AscToUnicode(str);
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->SetText(StrLeft);
 
 }
 void CashierWnd::AddGoodsCount() {
	//库存----减
 	CEditUI* pGoodsLeft = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT"));
 	CDuiString StrLeft = pGoodsLeft->GetText();
 	if (StrLeft == _T("0")) {
 		MessageBox(m_hWnd, _T("库存不足"), _T("警告"), IDOK);
 		return;
 	}
 	int count = atoi(UnicodeToAsc(StrLeft).c_str());
 	--count;
 	StrLeft.Format(_T("%d"), count);
 	pGoodsLeft->SetText(StrLeft);
 
 	//商品++++加
 	CEditUI* pGoodsCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_COUNT"));
 	CDuiString StrCount = pGoodsCount->GetText();
 	count = atoi(UnicodeToAsc(StrCount).c_str());
 	++count;
 	StrCount.Format(_T("%d"), count);
 	pGoodsCount->SetText(StrCount);
 }
 void CashierWnd::SubGoodsCount() {
 	//商品减一
 	CEditUI* pGoodsCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_COUNT"));
 	CDuiString StrCount = pGoodsCount->GetText();
 	if (StrCount == _T("0")) {
 		MessageBox(m_hWnd, _T("商品个数已经为0"), _T("警告"), IDOK);
 		return;
 	}
 	int count = atoi(UnicodeToAsc(StrCount).c_str());
 	--count;
 	StrCount.Format(_T("%d"), count);
	pGoodsCount->SetText(StrCount);
 	//库存加一
 	CEditUI* pGoodsLeft = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT"));
 	CDuiString StrLeft = pGoodsLeft->GetText();
 	count = atoi(UnicodeToAsc(StrLeft).c_str());
 	++count;
 	StrLeft.Format(_T("%d"), count);
 	pGoodsLeft->SetText(StrLeft);
 }
 void CashierWnd::InsertGoodsList() {
 	//获取商品名称以及购买数量
 	CDuiString StrGoodName = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->GetText();
 	CEditUI* pGoodCount = (CEditUI*)m_PaintManager.FindControl(_T("EDIT_COUNT"));
 	CDuiString StrCount = pGoodCount->GetText();
 
 	//从数据库获取商品信息
 	string StrSQL("select * from goods where GoodsName='");
 	StrSQL += UnicodeToAsc(StrGoodName);
 	StrSQL += "';";
 	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
 
 	//计算总价格
 	int count = atoi(UnicodeToAsc(StrCount).c_str());
 	double Price = atof(vRet[0][5].c_str());
 	Price = count * Price;
 	CDuiString StrPrice;
 	StrPrice.Format(_T("%lf"), Price);

 	//对List控件进行更新
 	CListTextElementUI* pItem = new CListTextElementUI;
 	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListGoodsInfo"));
 	pList->Add(pItem);
 	pItem->SetText(0, StrGoodName);
 	pItem->SetText(1, AscToUnicode(vRet[0][5]));
 	pItem->SetText(2, StrCount);
 	pItem->SetText(3, AscToUnicode(vRet[0][6]));
 	pItem->SetText(4, StrPrice);
	//将编辑框清空
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_GOODSNAME")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->SetText(_T(""));
	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_COUNT")))->SetText(_T(""));
 }
 void CashierWnd::CommiOrder() {
	//合计价格
 	CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListGoodsInfo"));
 	int count = pList->GetCount();
 
 	double Price = 0;
 	for (int i = 0; i < count; ++i) {
		CListTextElementUI* pItem = (CListTextElementUI*)pList->GetItemAt(i);
 		CDuiString StrPrice = pItem->GetText(4);
 		Price += atof(UnicodeToAsc(StrPrice).c_str());
 	}
 	CDuiString StrPrice;
 	StrPrice.Format(_T("%.02lf"), Price);
 	((CEditUI*)m_PaintManager.FindControl(_T("EDIT_MONEY")))->SetText(StrPrice);

	//更新商品数据库
 	for (int i = 0; i < count; ++i) {
		CListTextElementUI* pItem = (CListTextElementUI*)pList->GetItemAt(i);
 		CDuiString StrCount = pItem->GetText(2);
		CDuiString StrName = pItem->GetText(0);
		//CDuiString StrLeft = ((CEditUI*)m_PaintManager.FindControl(_T("EDIT_LEFT")))->GetText();

		//提取数据库中商品的库存
		string StrSQLLeft("select * from goods where GoodsName='");
		StrSQLLeft += UnicodeToAsc(StrName);
		StrSQLLeft += "';";
		vector<vector<string>> vRet = p_mysql->Select(StrSQLLeft);
		string StrLeft = vRet[0][7];
		//对库存进行更新,减去被买走的商品个数
		const char* str = StrLeft.c_str();
		int temp = atoi(str);
		int count = atoi((UnicodeToAsc(StrCount)).c_str());
		temp -= count;
		char buf[32] = { 0 };
		_itoa(temp, buf, 10);
		StrLeft = buf;
		//将新库存更新进数据库
 		string StrSQLUpdate("update goods set Inventory=");
		StrSQLUpdate += StrLeft;
		StrSQLUpdate += " where GoodsName='";
		StrSQLUpdate += UnicodeToAsc(StrName);
		StrSQLUpdate += "';";
 		p_mysql->Update(StrSQLUpdate);

 	}
	pList->RemoveAll();
 	//插入本次销售记录
 }

 //取消在List控件中的商品

 void CashierWnd::CancelGoods() {

	 CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("ListGoodsInfo"));
	 //方法
	 int RowNum = pList->GetCurSel();
	 //CListTextElementUI* pRow = (CListTextElementUI*)pList->GetItemAt(RowNum);
	 pList->RemoveAt(RowNum);
 }


 void CashierWnd::Notify(TNotifyUI& msg) {
	 //获取控件名字
	 CDuiString StrName = msg.pSender->GetName();

	 //根据控件类型响应消息
	 if (msg.sType == _T("click")) {
		 if (StrName == _T("BTN_CLOSE")) {
			 Close();
		 }
		 else if (StrName == _T("BTN_MIN")) {
			 SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		 }
		 else if (StrName == _T("BTN_SELECT")) {
			 SelectGoods();
		 }
		 else if (StrName == _T("BTN_ADD")) {
			 AddGoodsCount();
		 }
		 else if (StrName == _T("BTN_SUB")) {
			 SubGoodsCount();
		 }
		 else if (StrName == _T("BTN_ADDTO")) {
			 InsertGoodsList();
		 }
		 else if (StrName == _T("BTN_COMMIT")) {
			 CommiOrder();
		 }
		 else if (StrName == _T("BTN_CANCEL")) {
			 CancelGoods();
		 }
	 }
 }