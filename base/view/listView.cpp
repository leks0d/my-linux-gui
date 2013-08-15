#include "mango.h"

namespace mango
{

	ListViewItem::ListViewItem()
	{

	}
	

	ListViewItem::~ListViewItem()
	{

	}

	ListView::ListView(void)
	{

	}


	ListView::ListView(const TCHAR* title, View* parent, Rect* rect, int style, int show)
		: View(title, parent, rect, style, show)
	{

	}

	ListView::~ListView(void)
	{

	}


	//ɾ��LVITEM����
	BOOL ListView::deleteRecordItem(ListViewItem* lvitem)
	{
		if (lvitem->mask & LVIF_TEXT)
		{
			safeFree (lvitem->pszText) ;
		}

		return TRUE ;
	}


	//ɾ��һ�е���������
	BOOL ListView::deleteRecordAllItem(LISTVIEW_RECORD*  pItem)
	{
		LISTVIEW_RECORD_ITEM*	recordItem ;
		LIST_HEAD	*pos ;

		do
		{
			if (list_empty(&(pItem->m_listSub)))
				break;

			pos = pItem->m_listSub.next;
			recordItem = list_entry(pos, LISTVIEW_RECORD_ITEM, m_list) ;

			list_del (pos) ;
			deleteRecordItem (&(recordItem->m_lvItem)) ;
			safeFree (recordItem) ;

		} while (1) ;

		return TRUE ;
	}


	//ɾ��listview ��ָ������
	BOOL ListView::deleteRecord (LISTVIEW_RECORD*  pItem)
	{
		deleteRecordAllItem (pItem) ;
		deleteRecordItem (&(pItem->m_lvItem)) ;
		safeFree (pItem) ;

		return TRUE ;
	}


	//ɾ��listview ��������
	BOOL ListView::deleteAllRecord(void)
	{
		LISTVIEW_RECORD*	pItem ;
		LIST_HEAD	*pos ;

		do
		{

			if (list_empty(&mRecordHead))
				break;

			pos = mRecordHead.next;
			pItem = list_entry(pos, LISTVIEW_RECORD, m_list) ;

			list_del (pos) ;
			deleteRecord (pItem) ;

			if (mSelectedRecord == pItem)
				mSelectedRecord = NULL ;

		} while (1) ;	

		return TRUE ;
	}



	//��һ��
	LISTVIEW_RECORD*  ListView::getFirsetRecord (void)
	{
		LIST_HEAD			*pos ;
		LISTVIEW_RECORD*	pItem = NULL ;

		pos = mRecordHead.next ;
		if (pos != &(mRecordHead))
			pItem = list_entry(pos, LISTVIEW_RECORD, m_list) ;

		return pItem ;
	}


	//β��
	LISTVIEW_RECORD*  ListView::getTailRecord (void)
	{
		LIST_HEAD			*pos ;
		LISTVIEW_RECORD*	pItem = NULL ;

		pos = mRecordHead.next ;
		if (pos != &mRecordHead)
			pItem = list_entry(pos, LISTVIEW_RECORD, m_list) ;

		return pItem ;
	}


	//��һ��
	LISTVIEW_RECORD*  ListView::getNextRecord (LISTVIEW_RECORD* pItem, BOOL bCycle)
	{
		LIST_HEAD	*pos ;
		LISTVIEW_RECORD*  pNextItem = NULL ;

		if (pItem)
		{
			pos = pItem->m_list.next;
			if (pos != &(mRecordHead))
				pNextItem = list_entry(pos, LISTVIEW_RECORD, m_list) ;
		}
		else
			pNextItem = getFirsetRecord() ;

		return pNextItem ;
	}


	//��һ��
	LISTVIEW_RECORD*  ListView::getPrevRecord(LISTVIEW_RECORD* pItem, BOOL bCycle)
	{
		LIST_HEAD	*pos ;
		LISTVIEW_RECORD*  pPrevItem = NULL ;

		if (pItem)
		{
			pos = pItem->m_list.prev;
			if (pos != &(mRecordHead))
				pPrevItem = list_entry(pos, LISTVIEW_RECORD, m_list) ;
		}
		else
			pPrevItem = getTailRecord() ;

		return pPrevItem ;
	}


	//����˳��, ���node
	LISTVIEW_RECORD*  ListView::getRecord ( int index)
	{
		LIST_HEAD			*pos ;
		LISTVIEW_RECORD*	pItem = NULL ;

		if (index < 0)
			return NULL ;


		list_for_each (pos, &(mRecordHead))
		{
			if (index <= 0)
				break ;
			index-- ;
		}

		if (pos != &(mRecordHead))
			pItem = list_entry(pos, LISTVIEW_RECORD, m_list) ;

		return pItem ;
	}


	//����node, ���
	int  ListView::getIndex ( LISTVIEW_RECORD* pItem)
	{
		int			index = 0;
		LIST_HEAD	*pos ;


		if (pItem == NULL)
			return -1 ;

		list_for_each (pos, &(mRecordHead))
		{
			if (pos == &(pItem->m_list))
				break ;
	
			index++ ;
		}

		if (pos == &(pItem->m_list))
			return index ;
		else
			return -1;
	}


	//�����к�(�к� > 0), �������ָ��
	LISTVIEW_RECORD_ITEM*  ListView::getRecordItem (LISTVIEW_RECORD* pItem, int iRecordItem)
	{
		LISTVIEW_RECORD_ITEM* recordItem ;
		LIST_HEAD *pos ;

		if (iRecordItem < 1)
			return NULL ;

		pos = pItem->m_listSub.next ;
		while (pos != &(pItem->m_listSub))
		{
			recordItem = list_entry(pos, LISTVIEW_RECORD_ITEM, m_list) ;
			if (recordItem->m_lvItem.iSubItem == iRecordItem)
				break ;

			pos = pos->next;
		}

		if (pos)
			return recordItem ;
		else
			return NULL ;
	}


	//�Ű�, ȷ���������ʾλ��
	void ListView::layout()
	{
		LISTVIEW_RECORD*		pItem ;
		PCTRL_LISTVIEW_LAYOUT	pLayout ;

		Rect		rcClient ;
		int			x, y ;

		if (mNeetLayout == FALSE)
			return;

		getRect(rcClient) ;

		if (mStyle & (LVS_LIST | LVS_REPORT))
		{
			pLayout = &(mLayoutList) ;
			x = pLayout->m_rcMargin.top ;
			y = pLayout->m_rcMargin.left;

			pItem = getFirsetRecord() ;
			while (pItem)
			{
				pItem->m_rect.set(x, y, rcClient.right - pLayout->m_rcMargin.right, y + pLayout->m_sizeItem.cy) ;
				y += pLayout->m_sizeItem.cy + 6;//�Ӽ��7

				pItem = getNextRecord(pItem, FALSE);
			}

			mZoneSize.cx = rcClient.right - rcClient.left - pLayout->m_rcMargin.left - pLayout->m_rcMargin.right ;
			mZoneSize.cy = y + pLayout->m_rcMargin.bottom ;
		}
		else
		{
			pLayout = &(mLayoutIcon) ;
			x = pLayout->m_rcMargin.top ;
			y = pLayout->m_rcMargin.left;

			pItem = getFirsetRecord () ;
			while (pItem)
			{

				if (x + pLayout->m_sizeItem.cx + pLayout->m_sizeItemExtra.cx + pLayout->m_rcMargin.right > rcClient.right - rcClient.left)
				{
					y += pLayout->m_sizeItem.cy + pLayout->m_sizeItemExtra.cy ;
					x = pLayout->m_rcMargin.top ;
				}

				pItem->m_rect.set(x, y, x + pLayout->m_sizeItem.cx, y + pLayout->m_sizeItem.cy) ;
				x += pLayout->m_sizeItem.cx + pLayout->m_sizeItemExtra.cx ;

				pItem = getNextRecord (pItem, FALSE) ;
			}

			mZoneSize.cx = rcClient.right - rcClient.left - \
				pLayout->m_rcMargin.left - pLayout->m_rcMargin.right ;

			mZoneSize.cy = y + pLayout->m_rcMargin.bottom ;
		}

		mFirstPaintRecord = NULL ;
		mNeetLayout		 = FALSE ;

	}



	void ListView::getRecordRectInView (LISTVIEW_RECORD* pItem, Rect *lpRect)
	{
		Rect		rect ;
		PCTRL_LISTVIEW_LAYOUT pLayout ;

		pLayout	 = &(mLayoutIcon) ;
		rect     = pItem->m_rect ;

		rect.offset(mZonePoint.x, mZonePoint.y) ;

		if (!(mStyle & (LVS_LIST | LVS_REPORT)))
		{
			rect.left += (pLayout->m_sizeItem.cx - pLayout->m_sizeItemIcon.cx) / 2 ;
			rect.right  = rect.left + pLayout->m_sizeItemIcon.cx ; 
			rect.bottom = rect.top  + pLayout->m_sizeItemIcon.cy ;
		}

		*lpRect = rect ;
	}


	//����ȷzone �ڿͻ���������, ȷ��ָ���������ǲ��ݿ��Լ�
	BOOL ListView::ensureVisible (LISTVIEW_RECORD* pItem, BOOL bPartialOK)
	{
		int dy = 0 ;
		Rect  clientRect;
		getClientRect(clientRect);

		if (bPartialOK && \
			(pItem->m_rect.top + mZonePoint.y < clientRect.bottom) && \
			(pItem->m_rect.bottom + mZonePoint.y > clientRect.top))
		{
			return TRUE ;
		}

		if (pItem->m_rect.bottom + mZonePoint.y > clientRect.bottom)
		{
			dy = clientRect.bottom -	(pItem->m_rect.bottom + mZonePoint.y) ;
		}
		else if (pItem->m_rect.top + mZonePoint.y < clientRect.top)
		{
			dy = clientRect.top - (pItem->m_rect.top + mZonePoint.y) ;
		}

		mZonePoint.y += dy ;

		return TRUE ;
	}


	//���ҵ�һ����ʾ��
	LISTVIEW_RECORD* ListView::findFirstPainRecord (void)
	{
		LISTVIEW_RECORD*  pItem ;
		int					yPaint ;

		pItem = mFirstPaintRecord ;
		if (pItem == NULL)
			pItem = getFirsetRecord () ;

		if (pItem == NULL)
			return NULL ;

		yPaint = 0 - mZonePoint.y ;
		if (pItem->m_rect.top <= yPaint && pItem->m_rect.bottom > yPaint)
			return pItem ;

		else if (pItem->m_rect.top > yPaint)
		{
			//��Ҫ���ϲ���
			do
			{
				pItem = getPrevRecord (pItem, FALSE) ;
				if (pItem)
				{
					if (pItem->m_rect.top <= yPaint)
						break ;
				}
				else
				{
					pItem = getFirsetRecord () ;
					break ;
				}

			}while (1) ;
		}
		else
		{
			//��Ҫ���²���
			do
			{
				pItem = getNextRecord (pItem, FALSE) ;
				if (pItem)
				{
					if (pItem->m_rect.bottom > yPaint)
						break ;
				}
				else
				{
					pItem = getTailRecord () ;
					break ;
				}

			}while (1) ;
		}

		mFirstPaintRecord = pItem ;

		return pItem ;
	}


	//����x, y �����ҵ�ѡ����
	LISTVIEW_RECORD*  ListView::getRecordFromPoint(Point pt)
	{
		LISTVIEW_RECORD*  pItem;
		LISTVIEW_RECORD*  pItemPoint = NULL ;

		pItem = findFirstPainRecord();
		if (!pItem)
			return NULL ;

		//תΪZone ��������
		pt.x -= mZonePoint.x ;
		pt.y -= mZonePoint.y ;

		while (pItem)
		{
			if (pItem->m_rect.top > pt.y)
				break ;

			if (pItem->m_rect.ptIn(pt.x, pt.y))
			{
				pItemPoint = pItem ;
				break ;
			}

			pItem = getNextRecord (pItem, FALSE) ;
		}

		return pItemPoint ;
	}


	int ListView::onCreate()
	{
		Rect  clientRect;
		getClientRect(clientRect);


		INIT_LIST_HEAD (&(mRecordHead)) ;

		mStyle   = mStyle;
		mTextColor = RGB(0, 0, 0);	

		mLayoutList.m_hInstance   = NULL;
		mLayoutList.m_sizeItem.cx = clientRect.right - clientRect.left;
		mLayoutList.m_sizeItem.cy = 47 ;
		mLayoutList.m_sizeItemIcon.cx = 22 ;
		mLayoutList.m_sizeItemIcon.cy = 22 ;

		mLayoutList.m_rcMargin.set(0, 0, 0, 0);

		mLayoutIcon.m_hInstance  = NULL;
		mLayoutIcon.m_sizeItem.cx = 64 ;
		mLayoutIcon.m_sizeItem.cy = 64 ;
		mLayoutIcon.m_sizeItemIcon.cx = 48 ;
		mLayoutIcon.m_sizeItemIcon.cy = 48 ;

		mLayoutIcon.m_rcMargin.set(0, 0, 0, 0);

		mSelectedIndex = -1 ;
		return 0 ;
	}


	int	ListView::onDestroy()
	{
		deleteAllRecord();
		return 0;
	}



	//����LVITEM �е�mask ָ�������ֵ
	bool ListView::takeItem (ListViewItem* dest, ListViewItem* org)
	{
		int i ;

		if (org->mask & LVIF_IMAGE)
		{
			dest->iImage = org->iImage;
			dest->mask  |= LVIF_IMAGE;
		}

		if (org->mask & LVIF_ITEXT)
		{
			dest->iText = org->iText;
			dest->mask  |= LVIF_ITEXT;
		}

		if (org->mask & LVIF_PARAM)
		{
			dest->lParam = org->lParam;
			dest->mask  |= LVIF_PARAM;
		}

		if (org->mask & LVIF_STATE)
			dest->state = (dest->state & ~(org->stateMask)) | (org->state & org->stateMask);

		if (org->mask & LVIF_TEXT)
		{
			i = String::lstrlen (org->pszText);

			if (i + 1 > dest->cchTextMax)
			{
				safeFree (dest->pszText);
				dest->pszText = (TCHAR *)malloc ((i + 1) * sizeof (TCHAR)) ;
				if (dest->pszText == NULL)
				{
					dest->cchTextMax = 0 ;
					return FALSE ;
				}
				dest->cchTextMax = i + 1 ;
			}

			dest->mask  |= LVIF_TEXT ;
			String::copy (dest->pszText, org->pszText) ;
		}
		if(org->mask & LVIF_ADAPTER)
			dest->mask  |= LVIF_ADAPTER;
		dest->paramType = org->paramType;
		dest->iItem = org->iItem;
		return true  ;
	}


	//��ȡ��LVITEM ��mask ָ��������
	void ListView::retakeItem (ListViewItem* dest, ListViewItem* org)
	{
		int i ;

		if (dest->mask & LVIF_IMAGE)
			dest->iImage = org->iImage ;

		if (dest->mask & LVIF_ITEXT)
			dest->iText  = org->iText ;

		if (dest->mask & LVIF_PARAM)
			dest->lParam = org->lParam ;

		if (dest->mask & LVIF_STATE)
			dest->state = org->state ;

		if (dest->mask & LVIF_TEXT)
		{
			i = String::lstrlen (org->pszText) ;
			if (i + 1 < dest->cchTextMax)
				String::copy (dest->pszText, org->pszText) ;
		}
	}


	int ListView::insertItem (ListViewItem *item)
	{
		LISTVIEW_RECORD* record;
		LISTVIEW_RECORD* newRecord;
		int				 index = -1;
		if (item->iSubItem != 0)
			return -1;

		if (item->iItem < 0)
			return -1;

		if (!(item->mask & (LVIF_IMAGE | LVIF_ITEXT | LVIF_TEXT | LVIF_PARAM|LVIF_ADAPTER)))
			return -1;

		newRecord = (LISTVIEW_RECORD *)malloc(sizeof(LISTVIEW_RECORD));
		if (!newRecord)
			return -1;

		memset(newRecord, 0, sizeof (LISTVIEW_RECORD));
		INIT_LIST_HEAD(&(newRecord->m_listSub));

		takeItem(&(newRecord->m_lvItem), item) ;

		record = getRecord(item->iItem) ;
		if (record == NULL)
			list_add_tail(&(newRecord->m_list), &(mRecordHead));
		else
			list_add_tail(&(newRecord->m_list), &(record->m_list));

		index = 0 ; //list_order_prev (&(mRecordHead),  &(newRecord->m_list)) ;

		mNeetLayout = TRUE ;
		return index ;
	}


	int	ListView::setItem(ListViewItem *item)
	{
		LISTVIEW_RECORD*	record ;
		LISTVIEW_RECORD_ITEM*  recordItem;
		LISTVIEW_RECORD_ITEM*  newRecordItem ;

		if (item->iItem < 0 || item->iSubItem < 0)
			return FALSE ;

		record = getRecord (item->iItem) ;
		if (record == NULL)
			return FALSE ;

		if (item->iSubItem == 0)
		{
			takeItem (&(record->m_lvItem), item) ;
			return TRUE ;
		}

		//takeItem (&(pNewItem->m_lvItem), item) ;
		recordItem = getRecordItem (record, item->iSubItem) ;
		if (recordItem)
		{
			takeItem (&(recordItem->m_lvItem), item) ;
			return 1 ;
		}

		newRecordItem = (LISTVIEW_RECORD_ITEM*)malloc (sizeof (LISTVIEW_RECORD_ITEM)) ;
		if (!newRecordItem)
			return 0 ;

		memset (newRecordItem, 0, sizeof (LISTVIEW_RECORD_ITEM)) ;
		takeItem (&(newRecordItem->m_lvItem), item) ;
		list_add (&(record->m_list), &(newRecordItem->m_list)) ;

		mNeetLayout = TRUE ;
		return 1 ;
	}




	int	 ListView::deleteItem (int index)
	{
		LISTVIEW_RECORD*	record ;

		record = getRecord (index) ;
		if (record == NULL)
			return -1;

		list_del(&(record->m_list)) ;
		deleteRecord(record) ;

		mNeetLayout = TRUE;
		invalidateRect(NULL);
		return TRUE ;
	}


	int	 ListView::deleteAllItems(void)
	{
		deleteAllRecord();

		mNeetLayout = TRUE;
		mZonePoint.x = 0;
		mZonePoint.y = 0;

		return 0;
	}


	int	 ListView::getItem(ListViewItem *lvitem)
	{
		LISTVIEW_RECORD* record;

		record = getRecord(lvitem->iItem);
		if (record == NULL)
			return -1 ;

		retakeItem(lvitem, &record->m_lvItem);

		return 0;
	}


	int	 ListView::getItemCount(void)
	{
		int			count = 0 ;
		LIST_HEAD	*pos ;

		list_for_each (pos, &mRecordHead)
		{
			count++ ;
		}

		return count ;
	}


	int	 ListView::getHotItem(void)
	{
		int index = -1 ;

		if (mSelectedIndex >= 0)
			return mSelectedIndex ;

		if (mSelectedRecord)
			index = getIndex (mSelectedRecord) ;

		return index ;
	}


	int	ListView::getZoneY (void)
	{
		return  mZonePoint.y ;
	}


	//���� LVM_SETZONEY ��Ϣ
	int	 ListView::setZoneY(int y, bool update)
	{
		int oldY;

		oldY = mZonePoint.y;
		mZonePoint.y = y;

		invalidateRect(NULL);
		return oldY ;
	}


	int	 ListView::setItemState (int index, ListViewItem* lvitem)
	{
		LISTVIEW_RECORD*	record ;

		record = getRecord(index) ;
		if (record == NULL)
			return -1;

		record->m_lvItem.state = (record->m_lvItem.state & ~(lvitem->stateMask)) | (lvitem->state & lvitem->stateMask) ;
		invalidateRect(NULL);

		return 0;
	}


	//paint icon item
	bool ListView::paintIconRecord (Canvas& canvas, LISTVIEW_RECORD* record, bool eraseBk)
	{
		Rect rcText, rcItem, rcIcon ;
		ListViewItem* lvitem ;
		PCTRL_LISTVIEW_LAYOUT pLayout ;
		Brush		brush;

		rcItem   = record->m_rect ;
		lvitem = &(record->m_lvItem) ;
		pLayout	 = &(mLayoutIcon) ;

		rcItem.offset(mZonePoint.x, mZonePoint.y);

		rcIcon = rcItem ;
		rcIcon.left += (pLayout->m_sizeItem.cx - pLayout->m_sizeItemIcon.cx) / 2 ;
		rcIcon.right = rcIcon.left + pLayout->m_sizeItemIcon.cx ; 
		rcIcon.bottom = rcIcon.top += pLayout->m_sizeItemIcon.cy ;

		if (lvitem->mask & LVIF_IMAGE)
		{
			canvas.drawImageResource(lvitem->iImage, rcIcon.left, rcIcon.top);
		}
		else
		{
			//���տ�
			brush.set(0, RGB(255, 255, 255));
			canvas.frameRect(rcIcon, brush);

			rcIcon.inflate(-1, -1) ;
			brush.set(0, RGB(128, 128, 128));
			canvas.frameRect(rcIcon, brush);
		}

		rcText	   = rcItem ; 
		rcText.top = rcIcon.bottom ;

		if (lvitem->mask & LVIF_ITEXT)
		{
			canvas.drawTextResource(lvitem->iText, rcText, DT_CENTER | DT_VCENTER);
		}
		else if (lvitem->mask & LVIF_TEXT)
		{
			canvas.drawText(lvitem->pszText, -1, rcText, DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
		}

		return true;
	}


	//paint list item
	bool ListView::paintListRecord (Canvas& canvas, LISTVIEW_RECORD* record, bool eraseBk)
	{
		Rect rect;
		int	 x, y;
		ListViewItem* lvitem ;
		Brush 	brush ;

		rect     = record->m_rect ;
		lvitem = &(record->m_lvItem) ;

		rect.offset(mZonePoint.x, mZonePoint.y);
		
		if (record == mSelectedRecord){
			if(mPressItemBackground>0)
				canvas.drawImageResource(mPressItemBackground, 12, rect.top);
		}else{
			if(mItemBackground>0)
				canvas.drawImageResource(mItemBackground, 12, rect.top,false);			
		}
		canvas.setTextColor(RGB (255, 255, 255)) ;
		
		if (lvitem->mask & LVIF_ADAPTER){
			if(mListAdapter != NULL){
				//log_i("mListAdapter->PaintView lvitem->iItem=%d",lvitem->iItem);
				mListAdapter->PaintView(canvas,rect,lvitem,record == mSelectedRecord);
				return true;
			}
		}
//		log_i("ListView::paintListRecord left=%d,top=%d",rect.left,rect.top);
/*
		if (record == mSelectedRecord)
		{
			canvas.setTextColor(RGB (255, 255, 255)) ;
			brush.set(0, RGB(106, 197, 128));

			canvas.fillRect(rect, brush) ;
		}else
			
		*/

		if (eraseBk)
		{
			if (record == mSelectedRecord)
			{
				//	brush = CreateSolidBrush (RGBA (49, 106, 197, 128)) ;
				//	canvas.fillRect &rect, brush) ;
				//	DeleteObject ((HGDIOBJ)brush) ;
			}
			else
			{
				brush.set(0, RGB(255, 255, 255));
				//canvas.fillRect(rect, brush) ;
				log_i("ListView::paintListRecord ");
			}
		}

		if (lvitem->mask & LVIF_IMAGE)
		{
			x  = rect.left ;
			y  = (rect.bottom - rect.top - mLayoutList.m_sizeItemIcon.cy) / 2 ;
			y += rect.top ;
			if(lvitem->mask & LVIF_TEXT)
				canvas.drawImageResource(lvitem->iImage, 17, rect.top+5);
			else
				canvas.drawImageResource(lvitem->iImage, 97, y);
			rect.left += mLayoutList.m_sizeItemIcon.cx ;
		}

		
		if (lvitem->state & LVIS_ARROW)
		{
			x  = rect.right - mLayoutList.m_sizeItemIcon.cx ;
			y  = (rect.bottom - rect.top - mLayoutList.m_sizeItemIcon.cy) / 2 ;
			y += rect.top ;

			if (record == mSelectedRecord)
				canvas.drawImageResource(0, x, y);
			else
				canvas.drawImageResource(0, x, y);

			rect.right = x - 2 ;
		}

		if (lvitem->state & LVIS_ACTIVATING)
		{
			x  = rect.right - mLayoutList.m_sizeItemIcon.cx ;
			y  = (rect.bottom - rect.top - mLayoutList.m_sizeItemIcon.cy) / 2 ;
			y += rect.top ;

			canvas.drawImageResource(0, x, y);
			rect.right = x - 2 ;
		}

		if (lvitem->mask & LVIF_ITEXT)
		{
			rect.right -= 8 ;
			Rect it = rect;
			it.left = 125;
			it.right = it.left + 80;
			canvas.setTextSize(16);
			canvas.drawTextResource(lvitem->iText, it, DT_VCENTER);
		}
		else if (lvitem->mask & LVIF_TEXT)
		{
			rect.right -= 8 ;
			rect.left = 55;
			canvas.drawText(lvitem->pszText, -1, rect, DT_VCENTER | DT_END_ELLIPSIS);
		}

		if (record == mSelectedRecord)
			canvas.setTextColor(RGB (0, 0, 0)) ;

		return true;
	}


	//paint list item
	bool ListView::paintDragListItem (Canvas& canvas, LISTVIEW_RECORD* record)
	{
		return true ;
	}


	bool ListView::paintRecord (Canvas& canvas, LISTVIEW_RECORD* record, bool eraseBk)
	{
		//log_i("ListView::paintRecord");
		if (mStyle & (LVS_LIST | LVS_REPORT))
			return paintListRecord(canvas, record, eraseBk);
		else
			return paintIconRecord(canvas, record, eraseBk);
	}


	bool ListView::paintDragItem (Canvas& canvas, LISTVIEW_RECORD*	record)
	{
		return true;
	}


	int ListView::onPaint(Canvas& canvas)
	{
		//log_i("ListView::onPaint------------------------------------");

		LISTVIEW_RECORD* record;
		Rect  clientRect;
		getClientRect(clientRect);
		Brush brush(RGB(255, 255, 255));
		Rect rect;
		rect.setEx(0,0,320,212);
		layout();
		
		//canvas.fillRect(rect, brush);

		record = findFirstPainRecord();
		while (record)
		{
			//��Ҫ���²���
			if (record->m_rect.top + mZonePoint.y > clientRect.bottom)
				break;
			
			paintRecord(canvas, record, false);
			record = getNextRecord(record, false);
		}
		//log_i("ListView::onPaint---------------------end---------------");
		return 0 ;
	}



	int ListView::onKeyDown(int keyCode, int flag)
	{
		return 0 ;
	}


	int ListView::onKeyUp(int keyCode, int flag)
	{
		return 0 ;
	}


	int ListView::onTouchDown(int x, int y, int flag)
	{
		Point pt(x, y);

		layout();
		setCapture(this);

		mFocusedRecord = getRecordFromPoint (pt);
		if (mFocusedRecord)
		{
			mFocusedTime = (unsigned int)Time::getMillisecond();
			mSelectedRecord = mFocusedRecord ; 
		}
		invalidateRect(NULL);

		mTouchDownPosition.set(x, y);
		mTouchMove = false;

		return 0;
	}



	int ListView::onTouchMove(int x, int y, int flag)
	{
		Point pt(x, y);
		if (getCapture () != this)
			return 0 ;

		if (!mTouchMove) {
			if (mTouchDownPosition.y - y > 16 || mTouchDownPosition.y - y < -16)
				mTouchMove = true;

			mTouchPrevPosition.set(x, y);
		
		} else {
			cartoonDrag(y - mTouchPrevPosition.y);
			mTouchPrevPosition.set(x, y);
		}

		return 0 ;
	}


	int ListView::onTouchUp(int x, int y, int flag)
	{
		int	 index ;

		if (getCapture() != this)
			return 0 ;

		releaseCapture () ;
		
		mTouchPrevPosition.set(x, y);
		
		index =  getIndex(mFocusedRecord);
		if (index >= 0)
		{
			mSelectedIndex = index ;
			if (!mTouchMove)
				getParent()->onNotify(this, NM_CLICK, (void*)index);
		}

		mFocusedRecord = NULL ;
		mSelectedIndex = -1 ;
		return 0 ;
	}

	Point& ListView::getTouchPrevPosition(void){
		return mTouchPrevPosition;
	}
	void ListView::refresh(void){
		if(mListAdapter != NULL){
			mListAdapter->refresh();
		}
	}

	//����WM_LAYOUT ��Ϣ
	int	ListView::setLayout(PCTRL_LISTVIEW_LAYOUT layout)
	{
		if (mStyle & (LVS_LIST | LVS_REPORT))
		{
			mLayoutList.m_hInstance		= layout->m_hInstance ;
			mLayoutList.m_sizeItem		= layout->m_sizeItem ;
			mLayoutList.m_sizeItemIcon	= layout->m_sizeItemIcon ;
			mLayoutList.m_sizeItemExtra	= layout->m_sizeItemExtra ;
			mLayoutList.m_rcMargin		= layout->m_rcMargin ;
			mLayoutList.m_iImageActive  = layout->m_iImageActive ;
			mLayoutList.m_iImageArrow   = layout->m_iImageArrow ;
		}
		else
		{
			mLayoutIcon.m_hInstance		= layout->m_hInstance ;
			mLayoutIcon.m_sizeItem		= layout->m_sizeItem ;
			mLayoutIcon.m_sizeItemIcon	= layout->m_sizeItemIcon ;
			mLayoutIcon.m_sizeItemExtra	= layout->m_sizeItemExtra ;
			mLayoutIcon.m_rcMargin		= layout->m_rcMargin ;

		}
		mNeetLayout = TRUE ;

		return 0 ;
	}


	COLORREF ListView::setTextColor(COLORREF color)
	{
		COLORREF oldColor = mTextColor;

		mTextColor = color;

		return oldColor ;
	}



	//�����ַ���
	int ListView::insertString(int index, LPCTSTR lpszItem)
	{
		ListViewItem  lvItem ;

		lvItem.mask     = LVIF_TEXT ;
		lvItem.iItem    = index ;
		lvItem.iSubItem = 0 ;
		lvItem.pszText  = (LPTSTR)lpszItem ;

		return insertItem(&lvItem);
	}


	//�����ַ�����ͼƬID
	int ListView::insertStringImage(int index, LPCTSTR lpszItem, int iImage)
	{
		ListViewItem  lvItem ;

		lvItem.mask     = LVIF_TEXT | LVIF_IMAGE ;
		lvItem.iItem    = index ;
		lvItem.iSubItem = 0 ;
		lvItem.pszText  = (LPTSTR)lpszItem ;
		lvItem.iImage   = iImage ;

		return insertItem(&lvItem);
	}


	//�����ַ���ID��ͼƬID
	int ListView::insertTextIDImage (int index, int iText, int iImage)
	{
		ListViewItem  lvItem ;

		lvItem.mask     = LVIF_ITEXT | LVIF_IMAGE ;
		lvItem.iItem    = index ;
		lvItem.iSubItem = 0 ;
		lvItem.iText    = iText ;
		lvItem.iImage   = iImage ;

		return insertItem(&lvItem);
	}

	ListViewItem*  ListView::getSelectedItem(void)
	{
		if (!mSelectedRecord)
			return NULL;
		else
			return &(mSelectedRecord->m_lvItem);
	}


	//���ַ��ŶӲ���
	void ListView::binarySortInsert(LISTVIEW_RECORD** recordArray, int count, LISTVIEW_RECORD* record)
	{
		int low, mid, high ;
		int relationship ;
		int i ;

		low  = 0;
		high = count - 1;

		while (1)
		{
			mid = (low + high) / 2 ;

			relationship = compare(record, recordArray[mid]);

			//������ǰ��
			if (relationship < 0)
				high = mid - 1 ;
			else if (relationship > 0)
				low  = mid + 1 ;
			else
			{
				mid++ ;
				break ;
			}

			if (low > high)	{
				if (relationship > 0)
					mid++ ;
				break ;
			}
		}

		//Ϊ��Ԫ���ó��ռ�
		for (i = count ; i > mid ; i--)
			recordArray[i] = recordArray[i - 1] ;

		recordArray[mid] = record;
	}


	int	ListView::sort(void)
	{
		LISTVIEW_RECORD** recordArray;
		LISTVIEW_RECORD*  record;
		int count ;
		int i;

		count = getItemCount();
		if (count < 2)
			return 0;

		recordArray = (LISTVIEW_RECORD **)malloc (sizeof (LISTVIEW_RECORD *) * count) ;
		if (recordArray == NULL)
			return -1;

		record = getFirsetRecord() ;
		recordArray[0] = record ;

		for (i = 1; i < count; i++)	{
			record = getNextRecord(record);
			binarySortInsert (recordArray, i, record);
		}

		//��������
		for (i = 0 ; i < count ; i++) {
			record = recordArray[i];
			list_del(&(record->m_list)) ;
			list_add_tail(&(record->m_list), &mRecordHead) ;
		}

		safeFree(recordArray);
		return 0 ;
	}


	int ListView::compare(LISTVIEW_RECORD* reocrd1, LISTVIEW_RECORD* reocrd2)
	{
		return 1;
	}



	//�ָ�������ʾ�� ������ģʽ
	void ListView::resumeSelectedRecord(bool update)
	{
		if (mSelectedRecord)
		{
			mSelectedRecord = NULL;
			if (update)
				invalidateRect();
		}
	}


	//���� �ػ�����
	int  ListView::cartoonRedraw(Canvas& canvas, Rect& redrawRect)
	{
		Rect  clientRect;
		getClientRect(clientRect);

		Rect   rect, redrawZoneRect;
		LISTVIEW_RECORD* record;

		Brush brush(ARGB(255,0,0,0));

		canvas.fillRect(redrawRect, brush);

		redrawZoneRect = redrawRect;
		redrawZoneRect.offset(0, 0 - (mZonePoint.y + clientRect.top));

		record = findFirstPainRecord();
		while (record) {
			rect = redrawZoneRect;
			if (rect.intersect(record->m_rect))
				paintRecord (canvas, record, false);
			else if (record->m_rect.top > redrawZoneRect.bottom)
				break ;

			record = getNextRecord(record);
		}
		return 0 ;
	}



	int ListView::cartoonModifyDistance(int dy)
	{
		int  sideline ;
		Rect  clientRect;
		getClientRect(clientRect);

		if (dy == 0)
			return 0 ;

		//
		// ����ȷ��, �ƶ�����, ����Ҫ�ػ�����
		//
		if (dy < 0) {
			//���Ϲ���
			sideline = (clientRect.bottom - clientRect.top) / 3 ;
			if (mZoneSize.cy >= sideline * 2) {
				if (mZonePoint.y + dy + mZoneSize.cy < sideline)
					dy	= sideline - mZoneSize.cy - mZonePoint.y ;
			
			} else {
				if (mZonePoint.y + dy + mZoneSize.cy < 0)
					dy	= 0 - mZoneSize.cy - mZonePoint.y ;
			}

		} else {
			//���¹���
			sideline = (clientRect.bottom - clientRect.top) * 2 / 3 ;
			if (mZonePoint.y + dy > sideline) 
				dy = sideline - mZonePoint.y ;
		}
		return dy;
	}



	int ListView::cartoonMoveZone(int dy)
	{
		dy = cartoonModifyDistance(dy);
		mZonePoint.y  += dy ;
		return dy;
	}


	//���첢��ʾһ֡
	int ListView::cartoonDisplay(void)
	{
		Canvas* canvas;
		Rect  clientRect;
		getClientRect(clientRect);

		canvas = getCanvas();
		cartoonRedraw(*canvas, clientRect);
		canvas->swapScreenFrontBuffer();
		releaseCanvas();

		return 0;
	}


	//�϶�����
	int ListView::cartoonDrag(int distance)
	{
		int	dy ;
		Rect  clientRect;
		getClientRect(clientRect);

		resumeSelectedRecord();

		if (distance < 0 && mZoneSize.cy + mZonePoint.y + distance < clientRect.bottom)
			dy = distance  * 2 / 3 ;
		else if (mZonePoint.y > 0)
			dy = distance  * 2 / 3 ;
		else
			dy = distance ;

		if (cartoonMoveZone(dy))
#if 1			
			cartoonDisplay();
#else
			invalidateRect();
#endif
		return 0 ;
	}



};
