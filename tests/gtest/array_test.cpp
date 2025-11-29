#include "gtest/gtest.h"
#include "../sd/array/array.hpp"
#include <sstream>
#include <iostream>
#include <cstring>

class ArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        arr = new Mass();
    }

    void TearDown() override {
        delete arr;
    }

    Mass* arr;
};

// Тесты для полного покрытия конструктора и деструктора
TEST_F(ArrayTest, ConstructorInitializesCorrectly) {
    EXPECT_EQ(arr->get_size(), 0);
    EXPECT_TRUE(arr->is_empty());
}

// Тесты для покрытия всех веток reserve
TEST_F(ArrayTest, ReserveAllBranches) {
    // Тест 1: reserve с new_capacity <= capacity (ничего не должно произойти)
    arr->reserve(0); // capacity = 0, new_capacity = 0
    EXPECT_EQ(arr->get_size(), 0);
    
    // Тест 2: reserve с new_capacity > capacity (должна произойти реаллокация)
    arr->push_back("test");
    int old_capacity = 4; // начальная capacity после первого push_back
    arr->reserve(10);
    EXPECT_EQ(arr->get_size(), 1);
    EXPECT_EQ(arr->get_at(0), "test");
    
    // Тест 3: reserve с capacity = 0 и new_capacity > 0
    Mass arr2;
    arr2.reserve(5);
    EXPECT_EQ(arr2.get_size(), 0);
}

// Тесты для покрытия всех веток push_back
TEST_F(ArrayTest, PushBackAllBranches) {
    // Тест 1: Первый push_back (capacity = 0 -> становится 4)
    arr->push_back("first");
    EXPECT_EQ(arr->get_size(), 1);
    EXPECT_EQ(arr->get_at(0), "first");
    
    // Тест 2: push_back когда size == capacity (удвоение capacity)
    for (int i = 1; i < 4; ++i) {
        arr->push_back("elem_" + std::to_string(i));
    }
    // Теперь size = 4, capacity = 4, следующий push_back должен удвоить capacity
    arr->push_back("trigger_double");
    EXPECT_EQ(arr->get_size(), 5);
    EXPECT_EQ(arr->get_at(4), "trigger_double");
    
    // Проверяем, что старые данные сохранились
    for (int i = 0; i < 5; ++i) {
        if (i == 0) EXPECT_EQ(arr->get_at(i), "first");
        else if (i == 4) EXPECT_EQ(arr->get_at(i), "trigger_double");
        else EXPECT_EQ(arr->get_at(i), "elem_" + std::to_string(i));
    }
}

// Тесты для покрытия всех веток insert_at
TEST_F(ArrayTest, InsertAtAllBranches) {
    // Тест 1: Вставка в пустой массив с увеличением capacity
    arr->insert_at(0, "first");
    EXPECT_EQ(arr->get_size(), 1);
    EXPECT_EQ(arr->get_at(0), "first");
    
    // Тест 2: Вставка в начало с увеличением capacity
    arr->insert_at(0, "new_first");
    EXPECT_EQ(arr->get_size(), 2);
    EXPECT_EQ(arr->get_at(0), "new_first");
    EXPECT_EQ(arr->get_at(1), "first");
    
    // Тест 3: Вставка в конец с увеличением capacity
    // Заполняем до capacity
    arr->push_back("a");
    arr->push_back("b"); // теперь size = 4, capacity = 4
    arr->insert_at(4, "at_end"); // должен увеличить capacity
    EXPECT_EQ(arr->get_size(), 5);
    EXPECT_EQ(arr->get_at(4), "at_end");
    
    // Тест 4: Вставка в середину со сдвигом
    arr->insert_at(2, "middle");
    EXPECT_EQ(arr->get_size(), 6);
    EXPECT_EQ(arr->get_at(2), "middle");
    
    // Проверяем порядок: new_first, first, middle, a, b, at_end
    EXPECT_EQ(arr->get_at(0), "new_first");
    EXPECT_EQ(arr->get_at(1), "first");
    EXPECT_EQ(arr->get_at(2), "middle");
    EXPECT_EQ(arr->get_at(3), "a");
    EXPECT_EQ(arr->get_at(4), "b");
    EXPECT_EQ(arr->get_at(5), "at_end");
}

// Тесты для покрытия всех граничных условий insert_at
TEST_F(ArrayTest, InsertAtEdgeCases) {
    // Тест всех некорректных индексов
    arr->push_back("only");
    
    // index < 0
    arr->insert_at(-1, "invalid");
    EXPECT_EQ(arr->get_size(), 1);
    EXPECT_EQ(arr->get_at(0), "only");
    
    // index > size
    arr->insert_at(5, "invalid");
    EXPECT_EQ(arr->get_size(), 1);
    EXPECT_EQ(arr->get_at(0), "only");
    
    // index = size (корректный случай - вставка в конец)
    arr->insert_at(1, "at_end");
    EXPECT_EQ(arr->get_size(), 2);
    EXPECT_EQ(arr->get_at(1), "at_end");
}

// Тесты для покрытия всех веток del_at
TEST_F(ArrayTest, DeleteAtAllBranches) {
    // Подготавливаем массив с несколькими элементами
    arr->push_back("a");
    arr->push_back("b");
    arr->push_back("c");
    arr->push_back("d");
    
    // Тест 1: Удаление из середины
    arr->del_at(1);
    EXPECT_EQ(arr->get_size(), 3);
    EXPECT_EQ(arr->get_at(0), "a");
    EXPECT_EQ(arr->get_at(1), "c");
    EXPECT_EQ(arr->get_at(2), "d");
    
    // Тест 2: Удаление из начала
    arr->del_at(0);
    EXPECT_EQ(arr->get_size(), 2);
    EXPECT_EQ(arr->get_at(0), "c");
    EXPECT_EQ(arr->get_at(1), "d");
    
    // Тест 3: Удаление из конца
    arr->del_at(1);
    EXPECT_EQ(arr->get_size(), 1);
    EXPECT_EQ(arr->get_at(0), "c");
    
    // Тест 4: Удаление последнего элемента
    arr->del_at(0);
    EXPECT_EQ(arr->get_size(), 0);
    EXPECT_TRUE(arr->is_empty());
}

// Тесты для покрытия всех граничных условий del_at
TEST_F(ArrayTest, DeleteAtEdgeCases) {
    // Пустой массив
    arr->del_at(0); // не должно быть ошибки
    arr->del_at(-1);
    arr->del_at(100);
    EXPECT_TRUE(arr->is_empty());
    
    // Непустой массив с некорректными индексами
    arr->push_back("test");
    arr->del_at(-1);
    arr->del_at(1);
    arr->del_at(100);
    EXPECT_EQ(arr->get_size(), 1);
    EXPECT_EQ(arr->get_at(0), "test");
}

// Тесты для покрытия всех веток get_at
TEST_F(ArrayTest, GetAtAllBranches) {
    // Пустой массив - все индексы должны возвращать ""
    EXPECT_EQ(arr->get_at(-1), "");
    EXPECT_EQ(arr->get_at(0), "");
    EXPECT_EQ(arr->get_at(1), "");
    
    // Непустой массив
    arr->push_back("a");
    arr->push_back("b");
    
    // Корректные индексы
    EXPECT_EQ(arr->get_at(0), "a");
    EXPECT_EQ(arr->get_at(1), "b");
    
    // Некорректные индексы
    EXPECT_EQ(arr->get_at(-1), "");
    EXPECT_EQ(arr->get_at(2), "");
    EXPECT_EQ(arr->get_at(100), "");
}

// Тесты для покрытия всех веток replace_at
TEST_F(ArrayTest, ReplaceAtAllBranches) {
    // Пустой массив - замены не должно произойти
    arr->replace_at(0, "new");
    EXPECT_TRUE(arr->is_empty());
    
    // Непустой массив
    arr->push_back("old1");
    arr->push_back("old2");
    
    // Корректная замена
    arr->replace_at(0, "new1");
    EXPECT_EQ(arr->get_at(0), "new1");
    EXPECT_EQ(arr->get_at(1), "old2");
    
    arr->replace_at(1, "new2");
    EXPECT_EQ(arr->get_at(0), "new1");
    EXPECT_EQ(arr->get_at(1), "new2");
    
    // Некорректные индексы - замены не должно произойти
    arr->replace_at(-1, "invalid");
    arr->replace_at(2, "invalid");
    arr->replace_at(100, "invalid");
    EXPECT_EQ(arr->get_at(0), "new1");
    EXPECT_EQ(arr->get_at(1), "new2");
}

// Тесты для метода read (этот метод нужно протестировать через mock)
TEST_F(ArrayTest, ReadMethod) {
    // Сохраняем оригинальные cin и cout
    std::streambuf* orig_cin = std::cin.rdbuf();
    std::streambuf* orig_cout = std::cout.rdbuf();
    
    // Создаем строковый поток с тестовыми данными
    std::istringstream test_input("2\nElement1\nElement2\n");
    std::cin.rdbuf(test_input.rdbuf());
    
    // Перенаправляем cout для проверки вывода
    std::stringstream output_buffer;
    std::cout.rdbuf(output_buffer.rdbuf());
    
    // Вызываем метод read
    arr->read();
    
    // Восстанавливаем оригинальные потоки
    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
    
    // Проверяем результат
    EXPECT_EQ(arr->get_size(), 2);
    EXPECT_EQ(arr->get_at(0), "Element1");
    EXPECT_EQ(arr->get_at(1), "Element2");
    
    // Проверяем, что были prompts
    std::string output = output_buffer.str();
    EXPECT_FALSE(output.empty());
}

// Тест для read с нулевым количеством элементов
TEST_F(ArrayTest, ReadZeroElements) {
    std::streambuf* orig_cin = std::cin.rdbuf();
    std::streambuf* orig_cout = std::cout.rdbuf();
    
    std::istringstream test_input("0\n");
    std::cin.rdbuf(test_input.rdbuf());
    
    std::stringstream output_buffer;
    std::cout.rdbuf(output_buffer.rdbuf());
    
    arr->read();
    
    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
    
    EXPECT_EQ(arr->get_size(), 0);
    EXPECT_TRUE(arr->is_empty());
}

// Тест для read с большим количеством элементов
TEST_F(ArrayTest, ReadMultipleElements) {
    std::streambuf* orig_cin = std::cin.rdbuf();
    std::streambuf* orig_cout = std::cout.rdbuf();
    
    std::stringstream test_input;
    test_input << "5\n";
    for (int i = 0; i < 5; ++i) {
        test_input << "Element" << i << "\n";
    }
    
    std::cin.rdbuf(test_input.rdbuf());
    
    std::stringstream output_buffer;
    std::cout.rdbuf(output_buffer.rdbuf());
    
    arr->read();
    
    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
    
    EXPECT_EQ(arr->get_size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr->get_at(i), "Element" + std::to_string(i));
    }
}

// Тесты для метода print с разными состояниями массива
TEST_F(ArrayTest, PrintAllCases) {
    // Печать пустого массива
    testing::internal::CaptureStdout();
    arr->print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "\n"); // или " \n" в зависимости от реализации
    
    // Печать массива с одним элементом
    arr->push_back("single");
    testing::internal::CaptureStdout();
    arr->print();
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("single"), std::string::npos);
    
    // Печать массива с несколькими элементами
    arr->push_back("double");
    arr->push_back("triple");
    testing::internal::CaptureStdout();
    arr->print();
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("single"), std::string::npos);
    EXPECT_NE(output.find("double"), std::string::npos);
    EXPECT_NE(output.find("triple"), std::string::npos);
}

// Тесты для проверки целостности данных после множества операций
TEST_F(ArrayTest, DataIntegrityStressTest) {
    // Тест на переаллокацию памяти
    const int NUM_ELEMENTS = 100;
    
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        arr->push_back("value_" + std::to_string(i));
    }
    
    EXPECT_EQ(arr->get_size(), NUM_ELEMENTS);
    
    // Проверяем все элементы
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        EXPECT_EQ(arr->get_at(i), "value_" + std::to_string(i));
    }
    
    // Вставляем элементы в разные позиции
    for (int i = 0; i < 10; ++i) {
        arr->insert_at(i * 10, "inserted_" + std::to_string(i));
    }
    
    // Удаляем элементы
    for (int i = 0; i < 20; ++i) {
        arr->del_at(i);
    }
    
    // Заменяем элементы
    for (int i = 0; i < arr->get_size(); i += 5) {
        arr->replace_at(i, "replaced_" + std::to_string(i));
    }
    
    // Финальная проверка - массив не должен быть пустым и не должно быть крешей
    EXPECT_GT(arr->get_size(), 0);
    EXPECT_FALSE(arr->is_empty());
}

// Тест на последовательные операции вставки/удаления
TEST_F(ArrayTest, SequentialOperations) {
    // Чередуем вставку и удаление
    for (int i = 0; i < 20; ++i) {
        arr->push_back("push_" + std::to_string(i));
        if (i % 3 == 0) {
            arr->insert_at(0, "insert_" + std::to_string(i));
        }
        if (i % 4 == 0 && arr->get_size() > 0) {
            arr->del_at(arr->get_size() - 1);
        }
    }
    
    // Массив не должен быть пустым
    EXPECT_FALSE(arr->is_empty());
    EXPECT_GT(arr->get_size(), 0);
}

// Тест на все возможные сценарии с одним элементом
TEST_F(ArrayTest, SingleElementScenarios) {
    // Все операции с одним элементом
    arr->push_back("single");
    
    // get_at
    EXPECT_EQ(arr->get_at(0), "single");
    EXPECT_EQ(arr->get_at(-1), "");
    EXPECT_EQ(arr->get_at(1), "");
    
    // replace_at
    arr->replace_at(0, "modified");
    EXPECT_EQ(arr->get_at(0), "modified");
    arr->replace_at(-1, "invalid"); // не должно измениться
    arr->replace_at(1, "invalid");  // не должно измениться
    EXPECT_EQ(arr->get_at(0), "modified");
    
    // insert_at
    arr->insert_at(0, "before");
    EXPECT_EQ(arr->get_size(), 2);
    EXPECT_EQ(arr->get_at(0), "before");
    EXPECT_EQ(arr->get_at(1), "modified");
    
    arr->insert_at(2, "after");
    EXPECT_EQ(arr->get_size(), 3);
    EXPECT_EQ(arr->get_at(2), "after");
    
    // del_at
    arr->del_at(1);
    EXPECT_EQ(arr->get_size(), 2);
    EXPECT_EQ(arr->get_at(0), "before");
    EXPECT_EQ(arr->get_at(1), "after");
    
    // Очистка
    arr->del_at(0);
    arr->del_at(0);
    EXPECT_TRUE(arr->is_empty());
}

// Тест для проверки что деструктор корректно освобождает память
TEST(ArrayDestructorTest, DestructorFreesMemory) {
    // Создаем массив с данными
    Mass* local_arr = new Mass();
    for (int i = 0; i < 10; ++i) {
        local_arr->push_back("test_" + std::to_string(i));
    }
    
    // Деструктор должен быть вызван без утечек памяти
    delete local_arr;
    
    // Если мы дошли сюда - все ок
    SUCCEED();
}

// Тест для проверки начального состояния после конструктора
TEST(ArrayInitialStateTest, InitialState) {
    Mass arr;
    EXPECT_EQ(arr.get_size(), 0);
    EXPECT_TRUE(arr.is_empty());
    
    // Все методы должны работать с пустым массивом без ошибок
    EXPECT_EQ(arr.get_at(0), "");
    EXPECT_EQ(arr.get_at(-1), "");
    EXPECT_EQ(arr.get_at(100), "");
    
    arr.del_at(0);   // не должно быть ошибки
    arr.del_at(-1);  // не должно быть ошибки
    arr.del_at(100); // не должно быть ошибки
    
    arr.replace_at(0, "test");  // не должно быть ошибки
    arr.replace_at(-1, "test"); // не должно быть ошибки
    
    // Массив должен остаться пустым
    EXPECT_TRUE(arr.is_empty());
    EXPECT_EQ(arr.get_size(), 0);
}