

// JPEG XL Frame Stream
#include "fmtAll.hpp"
#include <iterator>
#include <cstdint>
#include <span>
#include <filesystem>
#include <ostream>
namespace jxls {

struct Frame {
    std::basic_string<u_int8_t> buffer; 
    int64_t id; 
};

class ReaderWriter
{
public:
    ReaderWriter(std::string_view strPath): 
        path(strPath),
        id(0)
    {
        if (!std::filesystem::exists(path)) throw std::runtime_error("file path doesn't exist");
        if (!std::filesystem::is_directory(path)) throw std::runtime_error("provided path is not a valid directory");
        

    }

    // reads frame from JPEG XL stream directory 
    std::basic_string<uint8_t> ReadFrame(uint64_t id) {

    }

    // insert JPEG XL frame at id in the stream directory
    bool InsertFrame(std::span<uint8_t> frame, uint64_t id) {
        std::ofstream fileStream; 
        fileStream.open(fmt::format("./frameseq/i{}.jxl", id), std::ios::out | std::ios::binary);
        fileStream.write(reinterpret_cast<const char*>(frame.data()), frame.size()); 
        fileStream.flush();
        fileStream.close();
    }
    
    // reads the next frame from the stream sequence
    std::basic_string<uint8_t> NextFrame() {
        return ReadFrame(++this->id);
    }

    // writes a new JPEG XL frame to the end of the stream sequence
    bool AddFrame(std::span<uint8_t> frame) {
        return this->InsertFrame(std::forward<std::span<uint8_t>>(frame), ++this->id); 
    }

    // set current frame id to `id`. if id > max existing id, then id will be set to max id 
    void Seek(uint64_t id) noexcept {
        this->id = id;
    }

private:
    std::filesystem::path path;
    std::filesystem::path metadata; 
    uint64_t id;
    uint64_t maxId; 
};

}