#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;

// Константи, які визначають розміри області та параметри руху
const int WIDTH = 100;     // Максимальна координата по осі X
const int HEIGHT = 30;     // Максимальна координата по осі Y
const int SPEED = 1;       // Швидкість переміщення (на кожному кроці координата змінюється на 1)
const int DELAY_MS = 100;  // Затримка між кроками в мілісекундах (100 мс)

// Мьютекс для синхронізованого виводу в консоль
mutex printMutex;

// Клас, що моделює рибу (або інший об’єкт, що рухається)
class Fish {
public:
    string name;        // Назва рибки (ідентифікатор)
    int x, y;           // Поточні координати
    int directionX;     // Напрямок руху по X: 1 = вправо, -1 = вліво, 0 = не рухається
    int directionY;     // Напрямок руху по Y: 1 = вниз, -1 = вгору, 0 = не рухається

    // Конструктор класу
    Fish(string name, int x, int y, int dx, int dy)
        : name(name), x(x), y(y), directionX(dx), directionY(dy) {
    }

    // Метод для переміщення по осі X
    void moveX() {
        while (true) {
            {
                // Блокуємо консоль для безпечного виводу
                lock_guard<mutex> lock(printMutex);
                cout << name << " at X=" << x << endl;
            }

            // Змінюємо координату x відповідно до напрямку
            x += directionX * SPEED;

            // Перевірка на межі області — змінюємо напрямок при зіткненні з краєм
            if (x >= WIDTH || x <= 0) {
                directionX *= -1; // Зміна напрямку руху на протилежний
            }

            // Затримка перед наступним кроком
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }

    // Метод для переміщення по осі Y
    void moveY() {
        while (true) {
            {
                // Блокуємо консоль для безпечного виводу
                lock_guard<mutex> lock(printMutex);
                cout << name << " at Y=" << y << endl;
            }

            // Змінюємо координату y відповідно до напрямку
            y += directionY * SPEED;

            // Перевірка на межі області — змінюємо напрямок при зіткненні з краєм
            if (y >= HEIGHT || y <= 0) {
                directionY *= -1; // Зміна напрямку руху на протилежний
            }

            // Затримка перед наступним кроком
            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }
};

int main() {
    vector<thread> threads; // Вектор потоків

    // Створення золотих рибок, які рухаються по осі X
    // Рух вправо від 0, рух вліво від WIDTH
    threads.emplace_back(&Fish::moveX, Fish("Goldfish_1", 0, 5, 1, 0));
    threads.emplace_back(&Fish::moveX, Fish("Goldfish_2", WIDTH, 10, -1, 0));

    // Створення гуппі, які рухаються по осі Y
    // Рух вниз від 0, рух вгору від HEIGHT
    threads.emplace_back(&Fish::moveY, Fish("Guppy_1", 20, 0, 0, 1));
    threads.emplace_back(&Fish::moveY, Fish("Guppy_2", 50, HEIGHT, 0, -1));

    // Очікування завершення потоків (оскільки цикли нескінченні — програма працюватиме постійно)
    for (auto& t : threads) {
        t.join(); // Блокує основний потік до завершення дочірнього
    }

    return 0;
}
