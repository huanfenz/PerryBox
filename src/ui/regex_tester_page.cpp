#include "regex_tester_page.h"
#include "ui_regex_tester_page.h"
#include "regex_tester.h"
#include <QComboBox>
#include <QPushButton>
#include <fmt/format.h>

using namespace perry;

/* 收集界面上的正则选项 */
static std::regex::flag_type currentFlags(const Ui::RegexTesterPage& ui)
{
    std::regex::flag_type flags = std::regex::ECMAScript;
    if (ui.cbox_regex_icase->isChecked()) {
        flags |= std::regex::icase;
    }
    return flags;
}

RegexTesterPage::RegexTesterPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegexTesterPage)
{
    ui->setupUi(this);

    // 用预设表填充下拉框
    for (const RegexPreset& preset : regexPresets()) {
        ui->cbox_regex_preset->addItem(QString::fromUtf8(preset.name));
    }

    // 匹配按钮事件
    connect(ui->btn_regex_match, &QPushButton::clicked, this, [this](){
        ui->label_info_regex->setText("");

        std::string pattern = ui->edit_regex_pattern->text().toStdString();
        if (pattern.empty()) {
            ui->label_info_regex->setText("正则表达式不能为空");
            return;
        }
        std::string text = ui->pedit_regex_input->toPlainText().toStdString();

        std::string error;
        auto results = regexMatch(pattern, text, currentFlags(*ui), &error);
        if (!error.empty()) {
            ui->label_info_regex->setText("正则表达式错误: " + QString::fromStdString(error));
            ui->edit_regex_result->setPlainText("");
            ui->label_regex_match_count->setText("匹配数：0");
            return;
        }

        // 拼接匹配结果文本
        std::string out;
        if (results.empty()) {
            out = "无匹配结果";
        } else {
            for (size_t i = 0; i < results.size(); ++i) {
                out += fmt::format("[{}] \"{}\"\n", i + 1, results[i].match);
                out += fmt::format("    位置: {}, 长度: {}\n", results[i].position, results[i].length);
            }
        }
        ui->edit_regex_result->setPlainText(QString::fromStdString(out));
        ui->label_regex_match_count->setText(QString::fromStdString(fmt::format("匹配数：{}", results.size())));
    });

    // 清空按钮事件
    connect(ui->btn_regex_clear, &QPushButton::clicked, this, [this](){
        ui->edit_regex_pattern->setText("");
        ui->pedit_regex_input->setPlainText("");
        ui->edit_regex_result->setPlainText("");
        ui->edit_regex_replace->setText("");
        ui->edit_regex_replace_result->setPlainText("");
        ui->label_info_regex->setText("");
        ui->label_regex_match_count->setText("匹配数：0");
        ui->cbox_regex_preset->setCurrentIndex(0);
    });

    // 替换按钮事件
    connect(ui->btn_regex_replace, &QPushButton::clicked, this, [this](){
        ui->label_info_regex->setText("");

        std::string pattern = ui->edit_regex_pattern->text().toStdString();
        if (pattern.empty()) {
            ui->label_info_regex->setText("正则表达式不能为空");
            return;
        }

        std::string text = ui->pedit_regex_input->toPlainText().toStdString();
        if (text.empty()) {
            ui->label_info_regex->setText("测试文本不能为空");
            return;
        }

        std::string replacement = ui->edit_regex_replace->text().toStdString();

        std::string error;
        std::string result = regexReplace(pattern, text, replacement, currentFlags(*ui), &error);
        if (!error.empty()) {
            ui->label_info_regex->setText("正则表达式错误: " + QString::fromStdString(error));
            return;
        }
        ui->edit_regex_replace_result->setPlainText(QString::fromStdString(result));
    });

    // 常用预设选择事件
    connect(ui->cbox_regex_preset, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        const auto& presets = regexPresets();
        if (index > 0 && index < static_cast<int>(presets.size())) {
            ui->edit_regex_pattern->setText(QString::fromUtf8(presets[index].pattern));
        }
    });
}

RegexTesterPage::~RegexTesterPage()
{
    delete ui;
}
