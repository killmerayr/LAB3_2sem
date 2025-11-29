#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include "../../sd/hash/hash.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

BOOST_AUTO_TEST_SUITE(HashSuite)

// Базовые тесты добавления, поиска и удаления
BOOST_AUTO_TEST_CASE(AddFindRemoveBasic)
{
    HashTable h;

    // Добавляем элементы
    BOOST_TEST(h.Add("apple"));
    BOOST_TEST(h.Add("banana"));
    BOOST_TEST(h.Add("cherry"));

    // Повторное добавление того же — должно вернуть false
    BOOST_TEST(!h.Add("apple"));
    BOOST_TEST(!h.Add("banana"));

    // Проверяем наличие
    BOOST_TEST(h.Find("apple"));
    BOOST_TEST(h.Find("banana"));
    BOOST_TEST(h.Find("cherry"));
    BOOST_TEST(!h.Find("durian"));

    // Удаляем
    BOOST_TEST(h.Remove("banana"));
    BOOST_TEST(!h.Find("banana"));
    BOOST_TEST(!h.Remove("banana")); // повторное удаление

    // Проверяем что остальные элементы на месте
    BOOST_TEST(h.Find("apple"));
    BOOST_TEST(h.Find("cherry"));
}

// Тесты конструктора и начального состояния
BOOST_AUTO_TEST_CASE(ConstructorTests)
{
    // Конструктор по умолчанию
    HashTable h1;
    BOOST_TEST(h1.get_size() == 0);
    
    // Конструктор с заданной capacity
    HashTable h2(16);
    BOOST_TEST(h2.get_size() == 0);
    
    // Проверка что таблица пустая
    BOOST_TEST(!h1.Find("anything"));
    BOOST_TEST(!h2.Find("test"));
}

// Тесты коллизий и линейного пробирования
BOOST_AUTO_TEST_CASE(CollisionHandling)
{
    HashTable h(4); // Маленькая таблица для гарантии коллизий
    
    // Добавляем элементы которые могут вызвать коллизии
    BOOST_TEST(h.Add("a"));
    BOOST_TEST(h.Add("b"));
    BOOST_TEST(h.Add("c"));
    
    // Проверяем что все элементы добавлены
    BOOST_TEST(h.Find("a"));
    BOOST_TEST(h.Find("b"));
    BOOST_TEST(h.Find("c"));
    
    // Удаляем элемент в середине цепочки
    BOOST_TEST(h.Remove("b"));
    BOOST_TEST(!h.Find("b"));
    BOOST_TEST(h.Find("a"));
    BOOST_TEST(h.Find("c"));
    
    // Добавляем новый элемент - должен занять место удаленного
    BOOST_TEST(h.Add("d"));
    BOOST_TEST(h.Find("d"));
    BOOST_TEST(h.Find("a"));
    BOOST_TEST(h.Find("c"));
}

// Тесты перехеширования
BOOST_AUTO_TEST_CASE(RehashingTest)
{
    HashTable h(4); // Начальная маленькая capacity
    
    // Добавляем элементы чтобы вызвать перехеширование
    // Порог: (size + deleted_count) >= capacity * 0.7
    BOOST_TEST(h.Add("key1"));
    BOOST_TEST(h.Add("key2"));
    BOOST_TEST(h.Add("key3")); // Должно вызвать перехеширование
    
    // Проверяем что все элементы доступны после перехеширования
    BOOST_TEST(h.Find("key1"));
    BOOST_TEST(h.Find("key2"));
    BOOST_TEST(h.Find("key3"));
    
    // Добавляем еще элементов
    BOOST_TEST(h.Add("key4"));
    BOOST_TEST(h.Add("key5"));
    BOOST_TEST(h.Add("key6"));
    
    // Все должны быть найдены
    BOOST_TEST(h.Find("key1"));
    BOOST_TEST(h.Find("key2"));
    BOOST_TEST(h.Find("key3"));
    BOOST_TEST(h.Find("key4"));
    BOOST_TEST(h.Find("key5"));
    BOOST_TEST(h.Find("key6"));
}

// Тесты с удаленными элементами (Deleted статус)
BOOST_AUTO_TEST_CASE(DeletedSlotHandling)
{
    HashTable h(8);
    
    // Добавляем и удаляем элементы для создания Deleted слотов
    BOOST_TEST(h.Add("temp1"));
    BOOST_TEST(h.Add("temp2"));
    BOOST_TEST(h.Add("perm"));
    
    BOOST_TEST(h.Remove("temp1"));
    BOOST_TEST(h.Remove("temp2"));
    
    // Deleted слоты должны использоваться при добавлении новых элементов
    BOOST_TEST(h.Add("new1"));
    BOOST_TEST(h.Add("new2"));
    
    BOOST_TEST(h.Find("perm"));
    BOOST_TEST(h.Find("new1"));
    BOOST_TEST(h.Find("new2"));
    BOOST_TEST(!h.Find("temp1"));
    BOOST_TEST(!h.Find("temp2"));
}

// Тесты граничных случаев
BOOST_AUTO_TEST_CASE(EdgeCases)
{
    HashTable h(4);
    
    // Удаление из пустой таблицы
    BOOST_TEST(!h.Remove("nonexistent"));
    
    // Поиск в пустой таблице
    BOOST_TEST(!h.Find("anything"));
    
    // Добавление пустой строки
    BOOST_TEST(h.Add(""));
    BOOST_TEST(h.Find(""));
    BOOST_TEST(h.Remove(""));
    BOOST_TEST(!h.Find(""));
}

// Тесты на полный обход таблицы (steps < capacity)
BOOST_AUTO_TEST_CASE(FullTableTraversal)
{
    HashTable h(4);
    
    // Заполняем таблицу почти полностью
    BOOST_TEST(h.Add("a"));
    BOOST_TEST(h.Add("b"));
    BOOST_TEST(h.Add("c"));
    
    // Поиск несуществующего элемента - должен пройти всю таблицу
    BOOST_TEST(!h.Find("nonexistent"));
    
    // Удаление несуществующего элемента
    BOOST_TEST(!h.Remove("nonexistent"));
}

// Тесты с большим количеством данных
BOOST_AUTO_TEST_CASE(StressTest)
{
    HashTable h;
    const int NUM_ELEMENTS = 100;
    
    // Добавляем много элементов
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Add(key));
        BOOST_TEST(h.Find(key));
    }
    
    // Проверяем что все элементы найдены
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Find(key));
    }
    
    // Удаляем каждый второй элемент
    for (int i = 0; i < NUM_ELEMENTS; i += 2) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Remove(key));
        BOOST_TEST(!h.Find(key));
    }
    
    // Проверяем оставшиеся элементы
    for (int i = 1; i < NUM_ELEMENTS; i += 2) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Find(key));
    }
    
    // Добавляем новые элементы - должны использовать Deleted слоты
    for (int i = NUM_ELEMENTS; i < NUM_ELEMENTS + 50; ++i) {
        std::string key = "newkey" + std::to_string(i);
        BOOST_TEST(h.Add(key));
        BOOST_TEST(h.Find(key));
    }
}

// Тесты хеш-функции (косвенно)
BOOST_AUTO_TEST_CASE(HashFunctionConsistency)
{
    HashTable h;
    
    // Одинаковые ключи должны иметь одинаковый хеш
    BOOST_TEST(h.Add("test"));
    BOOST_TEST(h.Find("test"));
    
    // Разные ключи (проверка что нет ложных срабатываний)
    BOOST_TEST(!h.Find("test2"));
    BOOST_TEST(!h.Find("Test")); // с учетом регистра
}

// Тесты метода Print (проверяем что не падает)
BOOST_AUTO_TEST_CASE(PrintMethod)
{
    HashTable h;
    
    // Печать пустой таблицы
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    h.Print();
    std::cout.rdbuf(old);
    // Не падает - успех
    
    // Печать непустой таблицы
    h.Add("print_test1");
    h.Add("print_test2");
    
    std::stringstream buffer2;
    old = std::cout.rdbuf(buffer2.rdbuf());
    h.Print();
    std::cout.rdbuf(old);
    
    std::string output = buffer2.str();
    BOOST_TEST(!output.empty());
}

// Тесты последовательных операций
BOOST_AUTO_TEST_CASE(SequentialOperations)
{
    HashTable h(8);
    
    // Добавить -> Найти -> Удалить -> Проверить удаление
    BOOST_TEST(h.Add("sequential"));
    BOOST_TEST(h.Find("sequential"));
    BOOST_TEST(h.Remove("sequential"));
    BOOST_TEST(!h.Find("sequential"));
    
    // Повторное добавление после удаления
    BOOST_TEST(h.Add("sequential"));
    BOOST_TEST(h.Find("sequential"));
}

// Тесты на специфические сценарии с коллизиями
BOOST_AUTO_TEST_CASE(SpecificCollisionScenarios)
{
    HashTable h(4);
    
    // Создаем цепочку коллизий
    BOOST_TEST(h.Add("collision1"));
    BOOST_TEST(h.Add("collision2"));
    BOOST_TEST(h.Add("collision3"));
    
    // Удаляем элемент из середины цепочки
    BOOST_TEST(h.Remove("collision2"));
    
    // Проверяем что остальные элементы доступны
    BOOST_TEST(h.Find("collision1"));
    BOOST_TEST(!h.Find("collision2"));
    BOOST_TEST(h.Find("collision3"));
    
    // Добавляем новый элемент - должен занять место удаленного в цепочке
    BOOST_TEST(h.Add("collision4"));
    BOOST_TEST(h.Find("collision4"));
    BOOST_TEST(h.Find("collision1"));
    BOOST_TEST(h.Find("collision3"));
}

// Тесты порога перехеширования
BOOST_AUTO_TEST_CASE(ResizeThreshold)
{
    HashTable h(10);
    
    // Добавляем элементы до порога перехеширования (0.7 * capacity = 7)
    for (int i = 0; i < 6; ++i) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Add(key));
    }
    
    // Следующее добавление должно вызвать перехеширование
    BOOST_TEST(h.Add("trigger_resize"));
    
    // Все элементы должны быть доступны
    for (int i = 0; i < 6; ++i) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Find(key));
    }
    BOOST_TEST(h.Find("trigger_resize"));
}

// Тесты с deleted_count в условии перехеширования
BOOST_AUTO_TEST_CASE(DeletedCountInResize)
{
    HashTable h(10);
    
    // Добавляем элементы
    for (int i = 0; i < 5; ++i) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Add(key));
    }
    
    // Удаляем некоторые элементы (увеличиваем deleted_count)
    for (int i = 0; i < 3; ++i) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Remove(key));
    }
    
    // Теперь size=2, deleted_count=3, total=5 что меньше 7 (0.7*10)
    // Добавляем еще элементы чтобы достичь порога
    for (int i = 5; i < 8; ++i) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Add(key));
    }
    
    // Проверяем что перехеширование произошло и все работает
    BOOST_TEST(h.Find("key3"));
    BOOST_TEST(h.Find("key4"));
    for (int i = 5; i < 8; ++i) {
        std::string key = "key" + std::to_string(i);
        BOOST_TEST(h.Find(key));
    }
}

// Тесты на циклический обход (index = (index + 1) % capacity)
BOOST_AUTO_TEST_CASE(CircularProbing)
{
    HashTable h(4);
    
    // Создаем ситуацию где probing должен пройти через конец таблицы
    BOOST_TEST(h.Add("a"));
    BOOST_TEST(h.Add("b"));
    BOOST_TEST(h.Add("c"));
    
    // Этот поиск должен пройти через всю таблицу циклически
    BOOST_TEST(!h.Find("nonexistent_end"));
    
    // Удаление также должно работать циклически
    BOOST_TEST(!h.Remove("nonexistent_end"));
}

// Тесты на стабильность после множества операций
BOOST_AUTO_TEST_CASE(StabilityAfterManyOperations)
{
    HashTable h;
    
    // Множественные добавления/удаления
    for (int cycle = 0; cycle < 10; ++cycle) {
        // Добавляем группу
        for (int i = 0; i < 10; ++i) {
            std::string key = "cycle" + std::to_string(cycle) + "_key" + std::to_string(i);
            BOOST_TEST(h.Add(key));
        }
        
        // Удаляем половину
        for (int i = 0; i < 5; ++i) {
            std::string key = "cycle" + std::to_string(cycle) + "_key" + std::to_string(i);
            BOOST_TEST(h.Remove(key));
        }
        
        // Проверяем оставшиеся
        for (int i = 5; i < 10; ++i) {
            std::string key = "cycle" + std::to_string(cycle) + "_key" + std::to_string(i);
            BOOST_TEST(h.Find(key));
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()