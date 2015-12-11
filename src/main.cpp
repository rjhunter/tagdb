#include <iostream>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <sqlite/connection.hpp>
#include <sqlite/execute.hpp>
#include <sqlite/query.hpp>
#include <taglib/tag.h>
#include <fileref.h>

using opts = std::pair<std::string, std::string>;

namespace {
    namespace fs = boost::filesystem;
    namespace po = boost::program_options;

    class TagDb {

        void scan_directory(const std::string &path);
        void build_database(const std::string &db_path);

        std::map<int, std::string> files;
        std::map<int, std::string> rev_artists;
        std::map<int, std::string> rev_albums;
        std::map<int, std::string> rev_titles;
        std::map<int, std::string> rev_genres;
        std::map<int, int> file_artist;
        std::map<int, int> file_album;
        std::map<int, int> file_title;
        std::map<int, int> file_track;
        std::map<int, int> file_genre;

    public:
        TagDb(const std::string& path, const std::string& db_path);
    };


    TagDb::TagDb(const std::string& path, const std::string& db_path) {
        scan_directory(path);
        build_database(db_path);
    }

    void TagDb::scan_directory(const std::string &path) {
        boost::filesystem::path p{path};
        boost::filesystem::recursive_directory_iterator rdi{p};
        boost::filesystem::recursive_directory_iterator end{};

        int file_counter{};
        int artist_counter{};
        int album_counter{};
        int title_counter{};
        int genre_counter{};

        std::map<std::string, int> artists;
        std::map<std::string, int> albums;
        std::map<std::string, int> titles;
        std::map<std::string, int> genres;

        for (auto it = rdi; it != end; ++it) {
            if (it->path().extension() == ".mp3") {
                auto fn = (*rdi).path().string();
                files[++file_counter] = fn;
            }
        }

        for (auto pair: files) {
            int file_id{pair.first};
            std::string file{pair.second};
            TagLib::FileRef f{file.c_str()};
            TagLib::Tag *tag{f.tag()};

            std::string artist{tag->artist().toCString()};
            std::string album{tag->album().toCString()};
            std::string title{tag->title().toCString()};
            std::string genre{tag->genre().toCString()};
            int track_id{static_cast<int>(tag->track())};

            if (artists.count(artist) == 0) {
                artists[artist] = ++artist_counter;
            }
            file_artist[file_id] = artists[artist];

            if (albums.count(album) == 0) {
                albums[album] = ++album_counter;
            }
            file_album[file_id] = albums[album];

            if (titles.count(title) == 0) {
                titles[title] = ++title_counter;
            }
            file_title[file_id] = titles[title];

            if (genres.count(genre) == 0) {
                genres[genre] = ++genre_counter;
            }
            file_genre[file_id] = genres[genre];

            file_track[file_id] = track_id;
        }

        for (auto pair: artists) {
            rev_artists[pair.second] = pair.first;
        }

        for (auto pair: albums) {
            rev_albums[pair.second] = pair.first;
        }

        for (auto pair: titles) {
            rev_titles[pair.second] = pair.first;
        }

        for (auto pair: genres) {
            rev_genres[pair.second] = pair.first;
        }


        std::cout << "files: " << files.size() << std::endl;
        std::cout << "artists: " << artists.size() << std::endl;
        std::cout << "albums: " << albums.size() << std::endl;
        std::cout << "titles: " << titles.size() << std::endl;
        std::cout << "genres: " << genres.size() << std::endl;
    }

    void TagDb::build_database(const std::string &db_path) {
        sqlite::connection con{db_path};
        sqlite::execute insert_artist{con, "INSERT INTO artist ('artist_id','name') VALUES (?,?);"};
        sqlite::execute insert_album{con, "INSERT INTO album ('album_id','name') VALUES (?,?);"};
        sqlite::execute insert_title{con, "INSERT INTO title ('title_id','name') VALUES (?,?);"};
        sqlite::execute insert_genre{con, "INSERT INTO genre ('genre_id','name') VALUES (?,?);"};
        sqlite::execute insert_file{con, "INSERT INTO file ('file_id','name','artist_id','album_id','title_id','track_id','genre_id') VALUES (?,?,?,?,?,?,?);"};

        for (auto pair: rev_artists) {
            insert_artist % pair.first % pair.second;
            insert_artist();
            insert_artist.clear();
        }

        for (auto pair: rev_albums) {
            insert_album % pair.first % pair.second;
            insert_album();
            insert_album.clear();
        }

        for (auto pair: rev_titles) {
            insert_title % pair.first % pair.second;
            insert_title();
            insert_title.clear();
        }

        for (auto pair: rev_genres) {
            insert_genre % pair.first % pair.second;
            insert_genre();
            insert_genre.clear();
        }

        for (auto pair: files) {
            int file_id{pair.first};
            std::string file{pair.second};
            int artist_id{file_artist[file_id]};
            int album_id{file_album[file_id]};
            int title_id{file_title[file_id]};
            int track_id{file_track[file_id]};
            int genre_id{file_genre[file_id]};

            insert_file % file_id % file % artist_id % album_id % title_id % track_id % genre_id;
            insert_file();
            insert_file.clear();
        }
    }

    opts parse_options(const int argc, const char** const argv) {
        // Declare the supported options.
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("music", po::value<std::string>(), "music directory (required)")
                ("database", po::value<std::string>(), "database file (required)")
                ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cerr << desc << std::endl;
            exit(0);
        }

        if (!vm.count("music")) {
            throw std::runtime_error{"Missing required path to music.  Use --help for more info."};
        }

        if (!vm.count("database")) {
            throw std::runtime_error{"Missing required database filename.  Use --help for more info."};
        }

        return std::pair<std::string, std::string>{
                vm["music"].as<std::string>(),
                vm["database"].as<std::string>()
        };
    }

}


int main(int argc, char* argv[])
{
    try {
        opts o = parse_options(argc, const_cast<const char** const>(argv));
        TagDb{o.first, o.second};
    } catch (std::exception& e) {
        std::cerr << e.what();
        exit(-1);
    } catch (...) {
        std::cerr << "Unknown exception caught." << std::endl;
        throw;
    }

    return 0;
}
