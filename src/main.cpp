#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QWidget>

#include <optional>
#include <vector>
#include <exception>
#include <format>

#include <fmt/core.h>

#include "failed_replay.hpp"


QPixmap load_image(const char* file)
{
    auto image = QPixmap(file);
    if (image.isNull()) throw std::runtime_error(fmt::format("couldn't find file: {}",file));

    return image;
}

class DifferenceDisplayer : public QWidget
{
public:
    virtual ~DifferenceDisplayer(){};
    DifferenceDisplayer(std::vector<FailedReplay> failed_replays, QWidget* parent = nullptr)
        : QWidget(parent), m_failed_replays(std::move(failed_replays))
    {
        this->setBaseSize(1280, 720);
        m_displayed_image = std::make_unique<QLabel>(this);
        update();
    }

protected:
    void keyPressEvent(QKeyEvent* event) override
    {
        switch (event->key())
        {
        case Qt::Key_Space:
            m_image_mode = m_image_mode == ORIGINAL ? REPLAY : ORIGINAL;
            break;
        case Qt::Key_Right:
            m_replay_index = std::clamp( m_replay_index + 1, 0, static_cast<int>(m_failed_replays.size() - 1));
            m_test_index = 0;
            break;
        case Qt::Key_Left:
            m_replay_index = std::clamp(m_replay_index - 1, 0, static_cast<int>(m_failed_replays.size() - 1));
            m_test_index = 0;
            break;
        case Qt::Key_Up:
            m_test_index = std::clamp(m_test_index + 1, 0, static_cast<int>(m_failed_replays[m_replay_index].failed_tests.size() - 1));
            break;
        case Qt::Key_Down:
            m_test_index = std::clamp(m_test_index - 1, 0, static_cast<int>(m_failed_replays[m_replay_index].failed_tests.size() - 1));
            break;
        }

        update();
    }

    void resizeEvent(QResizeEvent* event) override
    {
        update();
    }


private:
    void update()
    {
        auto& replay = m_failed_replays[m_replay_index];
        auto& test   = replay.failed_tests[m_test_index];

        auto width  = this->size().width();
        auto height = this->size().height();

        m_displayed_image->setGeometry(0, 0, width, height);
        auto image = load_image((m_image_mode == ORIGINAL ? test.original_image : test.replay_image).c_str());
        m_displayed_image->setPixmap(image.scaled(width, height));

        this->show();
        this->setWindowTitle(fmt::format("replay:{} image:{} ", replay.test_dir, m_image_mode == ORIGINAL ? test.original_image : test.replay_image).c_str());
    }

private:
    std::vector<FailedReplay> m_failed_replays;
    int m_replay_index = 0, m_test_index = 0;
    enum
    {
        ORIGINAL,
        REPLAY,
    } m_image_mode = ORIGINAL;
    std::unique_ptr<QLabel> m_displayed_image;
};

void foo();

int main(int argc, char** argv)
{
    foo();

    if (argc != 2) {
        fmt::print("please specify replay folder\n");
        
        
        return -1;
    }

    auto failed_replays = parse_json_failed_replays(fmt::format("{}/failed_replays.json",argv[1]).c_str());
    QApplication app(argc, argv);

    auto displayer = DifferenceDisplayer(std::move(failed_replays));

    return app.exec();
}


void foo()
{
    QString s = "testtesttesttesttesttest";
    
    std::string temp = fmt::format("{}", s.toLocal8Bit().data());
}