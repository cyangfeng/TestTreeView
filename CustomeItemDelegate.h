#pragma once
#include <QStyledItemDelegate>

enum ItemType
{
	Header = 0,
	Annot,
	Reply
};

enum ItemState
{
	None = 0,
	Editting,
	Replying
};

struct ItemInfo
{
	// 内容	
	QString wsIcon;
	QString wsAuthor;
	QString wsSubject;
	QString wsDateTime;
	
	bool bChecked = false;
	ItemType type = Header;
	ItemState state = None;

	// 相对位置
	QRect rcIcon;
	QRect rcAuthor;
	QRect rcMenuBtn;
	QRect rcSubject;
	QRect rcCheckBox;
	QRect rcDateTime;
};

Q_DECLARE_METATYPE(ItemInfo*)

class CustomeItemDelegate :
    public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit CustomeItemDelegate(QObject* parent = nullptr);
	~CustomeItemDelegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	
	// 更改尺寸
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	// 鼠标事件
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

private:
	// 定义交互区域的相对位置和大小
	QRect getInteractiveRect(const QStyleOptionViewItem& option) const;

private:
	void drawHeader(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void drawAnnotItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void drawReplyItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

	bool isFirstChild(const QModelIndex& index) const;
	bool isLastChild(const QModelIndex& index) const;
};

