#pragma once

#include <QWidget>
#include <QVBoxLayout>

class ListManager : public QWidget {
	Q_OBJECT
 public:
	explicit ListManager(QWidget *parent = nullptr);
 private slots:
	void update_things();
 private:
	QVBoxLayout *checkBoxesLayout;
};
