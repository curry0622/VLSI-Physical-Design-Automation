#include "Hardblock.hpp"

Hardblock::Hardblock() {
    name = "default";
    for(int i = 0; i < 4; i++) {
        cord[i] = Cord();
    }
    rotated = 0;
}

Hardblock::Hardblock(std::string name, Cord cord[]) {
    this->name = name;
    for(int i = 0; i < 4; i++) {
        this->cord[i] = cord[i];
    }
    rotated = 0;
}

Cord Hardblock::get_center() {
    int x = 0, y = 0;
    for(int i = 0; i < 4; i++) {
        x += cord[i].x;
        y += cord[i].y;
    }
    return Cord(x / 4, y / 4);
}

void Hardblock::rotate() {
    int width = cord[2].x - cord[0].x;
    int height = cord[2].y - cord[0].y;
    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;
    cord[1].y = cord[0].y + width;
    cord[2].x = cord[0].x + height;
    cord[2].y = cord[0].y + width;
    cord[3].x = cord[0].x + height;
    rotated = !rotated;
}

void Hardblock::print() {
    std::cout << "---" << std::endl;
    std::cout << "Hardblock name: " << name << std::endl;
    for(int i = 0; i < 4; i++) {
        std::cout << "Hardblock cord " << i << ": ";
        cord[i].print();
    }
    Cord center = get_center();
    std::cout << "Hardblock center: ";
    center.print();
    std::cout << "Hardblock rotated: " << rotated << std::endl;
    std::cout << "---" << std::endl;
}
