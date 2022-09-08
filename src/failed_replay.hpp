#include <string>
#include <vector>

struct FailedTest{
    std::string original_image,replay_image;
    double score;
};

struct FailedReplay{
    std::string test_dir;
    std::string replay_dir;
    std::vector<FailedTest> failed_tests;     
};

std::vector<FailedReplay> parse_json_failed_replays(std::string filename);
