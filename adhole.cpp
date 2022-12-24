#include "adhole.hpp"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QDir>
#include <QScrollArea>
#include <QWidget>
#include <QTextStream>
#include <QFile>
#include <QMap>
#include <QColor>
#include <QPalette>
#include <QMessageBox>
#include <compsky/os/write.hpp>
#include "systemd.hpp"

ListManager::ListManager(QWidget *parent)
: QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create a widget to hold the checkboxes
    QWidget *checkBoxesWidget = new QWidget(this);
    this->checkBoxesLayout = new QVBoxLayout(checkBoxesWidget);

    // Read the list of default-on files
    QStringList defaultOnFiles;
    QFile defaultOnFile("/home/vangelic/Documents/domain_checking.default-on.txt");
    if (defaultOnFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&defaultOnFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            defaultOnFiles << line;
        }
    }

    // Read the list of filename-to-color mappings
    QMap<QString, QColor> filenameToColor;
    QFile filenameToColorFile("/home/vangelic/Documents/domain_checking.filename_to_color.txt");
    if (filenameToColorFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&filenameToColorFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            int commaIndex = line.indexOf(',');
            if (commaIndex != -1) {
                QString colorName = line.left(commaIndex);
                QColor color(colorName);
                QString filename = line.mid(commaIndex + 1);
                filenameToColor[filename] = color;
            }
        }
    }

    QDir dir("/home/vangelic/Documents/domain_checking.py.modules");
    QStringList files = dir.entryList(QDir::Files);
    for (const QString &file : files) {
        QCheckBox *checkBox = new QCheckBox(file, checkBoxesWidget);

        // Set the checkbox state based on the default-on list
        checkBox->setCheckState(
			defaultOnFiles.contains(file) ?
			Qt::Checked :
			Qt::Unchecked
		);

        // Set the checkbox text color based on the filename-to-color mappings
        if (filenameToColor.contains(file)) {
            QColor color = filenameToColor[file];
            QPalette palette = checkBox->palette();
            palette.setColor(QPalette::WindowText, color);
            checkBox->setPalette(palette);
        }

        checkBoxesLayout->addWidget(checkBox);
    }

    // Add the widget with the checkboxes to a scroll area
	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setWidget(checkBoxesWidget);
	layout->addWidget(scrollArea);

	// Add the "Submit" button
	QPushButton *submitButton = new QPushButton("Submit", this);
	connect(submitButton, &QPushButton::clicked, this, &ListManager::update_things);
	layout->addWidget(submitButton);
}

void ListManager::update_things(){
	std::string x = "";
	QString selected_files = "";
	QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->checkBoxesLayout);
    for (int i = 0; i < layout->count(); ++i) {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(layout->itemAt(i)->widget());
        if (checkBox != nullptr && checkBox->checkState() == Qt::Checked) {
			selected_files += checkBox->text();
			selected_files += '\n';
			QString filename = QString("/home/vangelic/Documents/domain_checking.py.modules/%1").arg(checkBox->text());
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    if (!line.isEmpty() && !line.startsWith('#')) {
                        int commentIndex = line.indexOf(" #");
                        if (commentIndex != -1) {
                            line = line.left(commentIndex);
                        }
                        x += "server=/";
						x += line.toStdString();
						x += "/8.8.8.8\n";
                    }
                }
            }
        }
    }
    compsky::os::WriteOnlyFile f("/etc/dnsmasq.d/domain_checking.conf");
	f.write_from_buffer(x.c_str(), x.size());
	if (unlikely(restart_dnsmasq() != 0)){
		QMessageBox::warning(this, "Error", "Failed to restart dnsmasq");
	}
	
	const std::string y = selected_files.toStdString();
	compsky::os::WriteOnlyFile f2("/home/vangelic/Documents/domain_checking.default-on.txt");
	f2.write_from_buffer(y.c_str(), y.size());
}
