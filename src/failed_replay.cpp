#include "failed_replay.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <fmt/core.h>

#include "util.hpp"

std::vector<FailedReplay> parse_json_failed_replays(std::string file_name)
{
    for (auto& c : file_name)
        if (c == '\\') c = '/';

    auto file_data = read_file(file_name.c_str());

    auto json_doc = QJsonDocument::fromJson(QByteArray(file_data.data(), file_data.size()));

    auto replays_json = json_doc.array();

    auto file_dir = file_name.substr(0, file_name.rfind("/"));

    auto get_path = [&](const QString& s) {
        return fmt::format("{}/{}", file_dir, s.toLocal8Bit().data());
    };

    return map(replays_json, [&](QJsonValue&& replay_json) {
        auto obj = replay_json.toObject();
        return FailedReplay{
            .test_dir     = get_path(obj["TEST"].toString()),
            .replay_dir   = get_path(obj["REPLAY"].toString()),
            .failed_tests = map(obj["FAILED_TESTS"].toArray(), [&](QJsonValue&& test_json) {
                auto obj = test_json.toObject();
                return FailedTest{
                    .original_image = get_path(obj["ORIGINAL_IMAGE"].toString()),
                    .replay_image   = get_path(obj["REPLAY_IMAGE"].toString()),
                    .score          = obj["SCORE"].toDouble(),
                };
            }),
        };
    });
}
