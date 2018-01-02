#ifndef _EDITOR_FILE_H_
#define _EDITOR_FILE_H_

#include <string>

class File {

	private:

		std::string _content;

	public:

		void read(const char *file);
		const char *getContent() const;

        void insert(std::size_t pos, std::string str);
        void erase(std::size_t pos, std::size_t len);

        std::size_t getLength() const;
};

#endif
