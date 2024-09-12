#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QString>

class ListManager : public QWidget {
	Q_OBJECT
 public:
	explicit ListManager(QWidget* parent,  const char* const);
 private slots:
	void update_things();
 private:
	QVBoxLayout *checkBoxesLayout;
	QString qstr_settings_dir;
};
