#include "CustomeItemDelegate.h"
#include <QPainter>
#include <QEvent>
#include <QTreeView>
#include <QApplication>
#include <QCheckBox>
#include <QMouseEvent>
#include <QMenu>

CustomeItemDelegate::CustomeItemDelegate(QObject* parent)
{
}

CustomeItemDelegate::~CustomeItemDelegate()
{
}

void CustomeItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto itemInfo = index.data(Qt::UserRole).value<ItemInfo*>();
	if (!itemInfo)
		return;

	if (itemInfo->type == Header)
	{
		drawHeader(painter, option, index);
	}
	else if (itemInfo->type == Annot)
	{
		drawAnnotItem(painter, option, index);
	}
	else //Reply
	{
		drawReplyItem(painter, option, index);
	}
}

QSize CustomeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto itemInfo = index.data(Qt::UserRole).value<ItemInfo*>();
	if (itemInfo && itemInfo->type == Header)
	{
		return QSize(option.rect.width(), 50);
	} 

	if (itemInfo && itemInfo->state == Replying)
	{
		return QSize(option.rect.width(), 150);
	}

	return QSize(option.rect.width(), 100);
}

bool CustomeItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		// 鼠标左键
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->button() == Qt::LeftButton)
		{
			auto itemInfo = index.data(Qt::UserRole).value<ItemInfo*>();
			if (!itemInfo)
				return false;
			if (itemInfo->type == Header)
			{
				// 展开或者关闭当前项
				auto constTreeView = qobject_cast<const QTreeView*>(option.widget);
				if (constTreeView)
				{
					QTreeView* treeView = const_cast<QTreeView*>(constTreeView);
					if (treeView)
					{
						bool bExpaned = !treeView->isExpanded(index);
						treeView->setExpanded(index, bExpaned);
						// 展开/合上所有子项
						int nCount = index.model()->rowCount(index);
						for (int i = 0; i < nCount; i++)
						{
							QModelIndex childIndex = index.model()->index(i, 0, index);
							treeView->setExpanded(childIndex, bExpaned);
						}
						treeView->viewport()->update();
					}
					return true;
				}
			}
			else
			{
				// 处理复选框点击事件	
				QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
				// 判断是否点击在复选框区域
				auto pos = mouseEvent->pos();
				if (itemInfo->rcCheckBox.contains(mouseEvent->pos()))
					//if (getInteractiveRect(option).contains(mouseEvent->pos()))
				{
					bool bChecked = false;

					if (itemInfo)
					{
						bChecked = itemInfo->bChecked;
						itemInfo->bChecked = !bChecked;
					}

					// 更新视图
					auto treeView = qobject_cast<const QTreeView*>(option.widget);
					if (treeView)
					{
						treeView->viewport()->update();
					}
					return true;
				}
				else if (itemInfo->rcMenuBtn.contains(mouseEvent->pos()))
				{
					// 处理右键按下事件，弹出菜单
					QMenu menu;
					QAction* action1 = menu.addAction("Action 1");
					QAction* action2 = menu.addAction("Action 2");
					QAction* action3 = menu.addAction("Action 3");

					// 连接菜单项的信号到槽函数
					connect(action1, &QAction::triggered, [index]() {
						// 处理 Action 1
						qDebug() << "Action 1 triggered for index:" << index;
						});
					connect(action2, &QAction::triggered, [index]() {
						// 处理 Action 2
						qDebug() << "Action 2 triggered for index:" << index;
						});
					connect(action3, &QAction::triggered, [index]() {
						// 处理 Action 3
						qDebug() << "Action 3 triggered for index:" << index;
						});

					// 显示菜单
					menu.exec(mouseEvent->globalPosition().toPoint());
					return true;
				}
			}
		}
		else if (mouseEvent->button() == Qt::RightButton)
		{
			// 处理右键按下事件，弹出菜单
			QMenu menu;
			QAction* action1 = menu.addAction("Action 1");
			QAction* action2 = menu.addAction("Action 2");
			QAction* action3 = menu.addAction("Action 3");

			// 连接菜单项的信号到槽函数
			connect(action1, &QAction::triggered, [index]() {
				// 处理 Action 1
				qDebug() << "Action 1 triggered for index:" << index;
				});
			connect(action2, &QAction::triggered, [index]() {
				// 处理 Action 2
				qDebug() << "Action 2 triggered for index:" << index;
				});
			connect(action3, &QAction::triggered, [index]() {
				// 处理 Action 3
				qDebug() << "Action 3 triggered for index:" << index;
				});

			// 显示菜单
			menu.exec(mouseEvent->globalPosition().toPoint());
			return true;
		}
	}
	if (event->type() == QEvent::MouseButtonDblClick)
	{
		// TODO: 双击事件
		return true;
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);

}

QRect CustomeItemDelegate::getInteractiveRect(const QStyleOptionViewItem& option) const
{
	QRect rect = option.rect;
	int extraSpacing = 5; // 与 sizeHint 中的间隔一致
	rect.setTop(rect.top() + extraSpacing);
	rect.setBottom(rect.bottom() - extraSpacing);
	return rect;
}

void CustomeItemDelegate::drawHeader(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// 启用抗锯齿
	painter->setRenderHint(QPainter::Antialiasing, true);

	// 计算实际的绘制区域，减去底部的间隔
	QRect rect = option.rect;
	int extraSpacing = 3; // 与 sizeHint 中的间隔一致
	rect.setTop(rect.top() + extraSpacing);
	rect.setBottom(rect.bottom() - extraSpacing);
	rect.setLeft(rect.left() + extraSpacing);
	rect.setRight(rect.right() - extraSpacing);

	// 默认背景颜色，比子项略深
	QColor backgroundColor = QColor(200, 230, 255, 255);	
	// 检测项的状态
	//if (option.state & QStyle::State_MouseOver)
	//{
	//	// 悬停状态，改变背景颜色或添加效果
	//	backgroundColor = QColor(150, 150, 255, 255); // 悬停时的颜色
	//}
	//if (option.state & QStyle::State_Selected)
	//{
	//	// 选中状态，改变背景颜色或添加效果
	//	backgroundColor = QColor(100, 100, 255, 255); // 选中时的颜色
	//}

	// 绘制背景
	painter->setBrush(backgroundColor);
	painter->setPen(Qt::NoPen);
	painter->drawRoundedRect(rect, 10, 10);

	auto itemInfo = index.data(Qt::UserRole).value<ItemInfo*>();
	if (itemInfo)
	{
		// 绘制主题
		auto rcSubject = rect;
		rcSubject.setLeft(rect.left() + 10);
		painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
		painter->drawText(rcSubject, Qt::AlignLeft | Qt::AlignVCenter, itemInfo->wsSubject);
	}

	// 最右侧绘制“>”或“v”表示展开或收起
	auto treeView = qobject_cast<const QTreeView*>(option.widget);
	if (treeView)
	{
		int nTop = rect.top() + (rect.height() - 10) / 2 + extraSpacing;
		int nLeft = rect.right() - 30;
		QRect rcArrow(nLeft, nTop, 10, 10);
		QPoint points[3];
		if (treeView->isExpanded(index))
		{
			points[0] = rcArrow.topLeft();
			points[1] = QPoint((rcArrow.left() + rcArrow.right()) / 2, rcArrow.bottom());
			points[2] = QPoint(rcArrow.right(), rcArrow.top());
		}
		else
		{
			points[0] = rcArrow.topLeft();
			points[1] = QPoint(rcArrow.right(), (rcArrow.top() + rcArrow.bottom()) / 2);
			points[2] = QPoint(rcArrow.left(), rcArrow.bottom());
		}
		painter->drawPolyline(points, 3);
	}
}

void CustomeItemDelegate::drawAnnotItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto itemInfo = index.data(Qt::UserRole).value<ItemInfo*>();
	if (!itemInfo)
		return;

	auto treeView = qobject_cast<const QTreeView*>(option.widget);
	if (!treeView)
		return;
	// 启用抗锯齿
	painter->setRenderHint(QPainter::Antialiasing, true);

	// 计算实际的绘制区域，减去底部的间隔
	QRect rect = option.rect;
	int extraSpacing = 3; // 与 sizeHint 中的间隔一致
	rect.setTop(rect.top() + extraSpacing);
	if (!index.model()->hasChildren(index))
		rect.setBottom(rect.bottom() - extraSpacing);
	rect.setLeft(rect.left() + extraSpacing);
	rect.setRight(rect.right() - extraSpacing);

	painter->setClipRect(rect);

	//边框色
	QColor borderColor = QColor(255, 217, 227, 247);
	// 默认背景颜色
	QColor backgroundColor = QColor(200, 230, 230, 230);
	// 检测项的状态
	if (option.state & QStyle::State_MouseOver)
	{
		// 悬停状态，改变背景颜色或添加效果
		backgroundColor = QColor(150, 150, 255, 255); // 悬停时的颜色
	}
	if (option.state & QStyle::State_Selected)
	{
		// 选中状态，改变背景颜色或添加效果
		backgroundColor = QColor(100, 100, 255, 255); // 选中时的颜色
	}

	// 绘制背景
	// 若当前item有子项，下方开口
	if (index.model()->hasChildren(index))
	{
		// 设置剪切区域
		painter->setClipRect(rect);
		auto rcBackground = rect;
		rcBackground.setBottom(rect.bottom() + 15);
		painter->setBrush(backgroundColor);
		painter->setPen(borderColor);
		painter->drawRoundedRect(rcBackground, 10, 10);
		// 取消剪切区域
		painter->setClipping(false);
	}
	else
	{
		painter->setBrush(backgroundColor);
		painter->setPen(borderColor);
		painter->drawRoundedRect(rect, 10, 10);
	}

	// 计算各个元素的位置
	itemInfo->rcIcon = QRect(rect.left() + 10, rect.top() + 10, 20, 20);
	itemInfo->rcAuthor = QRect(itemInfo->rcIcon.right() + 10, rect.top() + 10, 100, 20);
	itemInfo->rcMenuBtn = QRect(rect.right() - 30, rect.top() + 10, 20, 20);
	itemInfo->rcSubject = QRect(itemInfo->rcAuthor.left(), itemInfo->rcAuthor.bottom() + 10, rect.width() - 20, 20);
	itemInfo->rcDateTime = QRect(rect.right() - 80, itemInfo->rcSubject.bottom() + 10, 80, 20);
	itemInfo->rcCheckBox = QRect(itemInfo->rcDateTime.left() - 10 - 16, itemInfo->rcDateTime.top() + 2, 16, 16);

	// 绘制椭圆icon
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawEllipse(itemInfo->rcIcon);

	// 绘制作者，与icon中心对齐
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawText(itemInfo->rcAuthor, Qt::AlignLeft | Qt::AlignVCenter, itemInfo->wsAuthor);

	// 绘制菜单按钮，三个点被矩形包围
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawText(itemInfo->rcMenuBtn, Qt::AlignCenter, "***");

	// 绘制主题
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawText(itemInfo->rcSubject, Qt::AlignLeft | Qt::AlignVCenter, itemInfo->wsSubject);

	// 绘制日期时间
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawText(itemInfo->rcDateTime, Qt::AlignLeft | Qt::AlignVCenter, itemInfo->wsDateTime);


	// 绘制复选框
	//QStyleOptionViewItem opt;
	//opt.rect = itemInfo->rcDateTime;
	//opt.state = QStyle::State_Enabled | QStyle::State_Active;
	//opt.state |= bChecked ? QStyle::State_On : QStyle::State_Off;
	//QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);

	// 绘制复选框，中间实心表示选中
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawRect(itemInfo->rcCheckBox.adjusted(2, 2, -2, -2));
	if (itemInfo->bChecked)
	{
		painter->setBrush(QBrush(QColor(0, 0, 0, 255)));
		painter->drawRect(itemInfo->rcCheckBox.adjusted(5, 5, -5, -5));
	}
}

void CustomeItemDelegate::drawReplyItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto itemInfo = index.data(Qt::UserRole).value<ItemInfo*>();
	if (!itemInfo)
		return;

	auto treeView = qobject_cast<const QTreeView*>(option.widget);
	if (!treeView)
		return;
	// 启用抗锯齿
	painter->setRenderHint(QPainter::Antialiasing, true);

	// 计算实际的绘制区域，减去底部的间隔
	QRect rect = option.rect;
	int extraSpacing = 3; // 与 sizeHint 中的间隔一致
	if (isLastChild(index))
	{
		//rect.setTop(rect.top() + extraSpacing);
		rect.setBottom(rect.bottom() - extraSpacing);
	}
	rect.setLeft(rect.left() + extraSpacing);
	rect.setRight(rect.right() - extraSpacing);

	//边框色
	QColor borderColor = QColor(255, 217, 227, 247);
	// 默认背景颜色
	QColor backgroundColor = QColor(200, 230, 230, 230);
	// 检测项的状态
	if (option.state & QStyle::State_MouseOver)
	{
		// 悬停状态，改变背景颜色或添加效果
		backgroundColor = QColor(150, 150, 255, 255); // 悬停时的颜色
	}
	if (option.state & QStyle::State_Selected)
	{
		// 选中状态，改变背景颜色或添加效果
		backgroundColor = QColor(100, 100, 255, 255); // 选中时的颜色
	}

	// 绘制背景
	// Annot的最后一个Reply，上开口和Annotation衔接
	if (isLastChild(index))
	{
		// 设置剪切区域
		painter->setClipRect(rect);
		auto rcBackground = rect;
		rcBackground.setTop(rect.top() - 15);
		painter->setBrush(backgroundColor);
		painter->setPen(borderColor);
		painter->drawRoundedRect(rcBackground, 10, 10);
		// 取消剪切区域
		painter->setClipping(false);
	}
	else
	{
		painter->setClipRect(rect);
		auto rcBackground = rect;
		rcBackground.adjust(0, -1, 0, 1);
		painter->setBrush(backgroundColor);
		painter->setPen(borderColor);
		painter->drawRect(rect);
		painter->setClipping(false);
	}

	// 计算各个元素的位置
	itemInfo->rcIcon = QRect(rect.left() + 10, rect.top() + 10, 20, 20);
	itemInfo->rcAuthor = QRect(itemInfo->rcIcon.right() + 20, rect.top() + 10, 100, 20);
	itemInfo->rcMenuBtn = QRect(rect.right() - 30, rect.top() + 10, 20, 20);
	itemInfo->rcSubject = QRect(itemInfo->rcAuthor.left(), itemInfo->rcAuthor.bottom() + 10, rect.width() - 20, 20);
	itemInfo->rcDateTime = QRect(rect.right() - 80, itemInfo->rcSubject.bottom() + 10, 80, 20);
	itemInfo->rcCheckBox = QRect(itemInfo->rcDateTime.left() - 10 - 16, itemInfo->rcDateTime.top() + 2, 16, 16);


	// 绘制作者，与icon中心对齐
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawText(itemInfo->rcAuthor, Qt::AlignLeft | Qt::AlignVCenter, itemInfo->wsAuthor);

	// 绘制菜单按钮，三个点被矩形包围
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawText(itemInfo->rcMenuBtn, Qt::AlignCenter, "***");

	// 绘制主题
	//painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	//painter->drawText(itemInfo->rcSubject, Qt::AlignLeft | Qt::AlignVCenter, itemInfo->wsSubject);

	// 绘制日期时间
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawText(itemInfo->rcDateTime, Qt::AlignLeft | Qt::AlignVCenter, itemInfo->wsDateTime);


	// 绘制复选框
	//QStyleOptionViewItem opt;
	//opt.rect = itemInfo->rcDateTime;
	//opt.state = QStyle::State_Enabled | QStyle::State_Active;
	//opt.state |= bChecked ? QStyle::State_On : QStyle::State_Off;
	//QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);

	// 绘制复选框，中间实心表示选中
	painter->setPen(QPen(QColor(0, 0, 0, 255), 1));
	painter->drawRect(itemInfo->rcCheckBox.adjusted(2, 2, -2, -2));
	if (itemInfo->bChecked)
	{
		painter->setBrush(QBrush(QColor(0, 0, 0, 255)));
		painter->drawRect(itemInfo->rcCheckBox.adjusted(5, 5, -5, -5));
	}
}

bool CustomeItemDelegate::isFirstChild(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return false;
	}

	return index.row() == 0;
}

bool CustomeItemDelegate::isLastChild(const QModelIndex& index) const
{
	if (!index.isValid()) 
	{
		return false;
	}

	QModelIndex parentIndex = index.parent();
	int rowCount = index.model()->rowCount(parentIndex);
	return index.row() == rowCount - 1;
}
