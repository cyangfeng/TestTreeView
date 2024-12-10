#include "TestTreeView.h"
#include "CustomeItemDelegate.h"
#include <QStandardItemModel>
#include <QStandardItem>

TestTreeView::TestTreeView(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

	QStringList strAuthor{ "Adminaster", "John_Doe", "Jane_Smith", "Alice_Johnson", "Robert_Brown", "Emily_Davis" };
	QStringList strSubjects{ "Highlight", "TypeWriter", "Area Highlight", "Rectangle", "Note", "Pencil"};

	QStandardItemModel* model = new QStandardItemModel(0, 1, this);
	QStandardItem* rootItem = model->invisibleRootItem();
	for (int i = 0; i < 100; i++)
	{
		QStandardItem* item = new QStandardItem(QString("Item %1").arg(i));
		auto itemInfo = new ItemInfo();
		itemInfo->type = Header;
		itemInfo->wsIcon = "icon";
		itemInfo->wsSubject = QString("Page %1").arg(i);
		item->setData(QVariant::fromValue(itemInfo), Qt::UserRole);
		rootItem->appendRow(item);

		for (int j = 0; j < 5; j++)
		{
			QStandardItem* childItem = new QStandardItem(QString("Child Item %1").arg(j));
			auto childItemInfo = new ItemInfo();
			childItemInfo->type = Annot;
			childItemInfo->wsIcon = "icon";
			childItemInfo->wsAuthor = strAuthor[std::rand() % 5];
			childItemInfo->wsSubject = strSubjects[std::rand() % 5];
			childItemInfo->wsDateTime = "2014.12.07";

			childItemInfo->bChecked = std::rand() % 2;

			childItem->setData(QVariant::fromValue(childItemInfo), Qt::UserRole);
			item->appendRow(childItem);

			// 添加Reply作为子项
			if (j == 4)
			{
				for (int k = 0; k < 5; k++)
				{
					QStandardItem* replyItem = new QStandardItem(QString("Child Item %1").arg(k));
					auto childItemInfo = new ItemInfo();
					childItemInfo->type = Reply;
					childItemInfo->wsIcon = "icon";
					childItemInfo->wsAuthor = strAuthor[std::rand() % 5];
					childItemInfo->wsDateTime = "2014.12.07";

					replyItem->setData(QVariant::fromValue(childItemInfo), Qt::UserRole);
					childItem->appendRow(replyItem);
				}
			}
		}
	}
	ui.treeView->setModel(model);
	ui.treeView->setItemDelegate(new CustomeItemDelegate(this));
	//ui.treeView->expandAll();
	ui.treeView->setIndentation(0);
	ui.treeView->setHeaderHidden(true);
	ui.treeView->setFrameStyle(QFrame::NoFrame);

	// 设置平滑滚动
	ui.treeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	// 链接信号槽，展开所有节点
	connect(ui.btn_expand_all, &QPushButton::clicked, ui.treeView, [this]() {
			ui.treeView->expandAll();
		});

	// 链接信号槽，折叠所有节点
	connect(ui.btn_collapse_all, &QPushButton::clicked, ui.treeView, [this]() {
		ui.treeView->collapseAll();
		});
}

TestTreeView::~TestTreeView()
{}
