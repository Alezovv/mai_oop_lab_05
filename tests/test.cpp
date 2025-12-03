#include <gtest/gtest.h>
#include "../include/List.h"
#include "../include/MemoryResource.h"
#include <vector>
#include <string>

// Тест 1: Базовые операции
TEST(DoublyListTest, PushBackAndIteration)
{
    MemoryResource mem(1024);
    DoublyLinkedList<int> list(&mem);

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    std::vector<int> v;
    for (auto x : list)
        v.push_back(x);

    ASSERT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

// Тест 2: Добавление в начало
TEST(DoublyListTest, PushFrontWorks)
{
    MemoryResource mem(1024);
    DoublyLinkedList<int> list(&mem);

    list.push_front(5);
    list.push_front(3);

    auto it = list.begin();
    ASSERT_NE(it, list.end());
    EXPECT_EQ(*it, 3);
    ++it;
    ASSERT_NE(it, list.end());
    EXPECT_EQ(*it, 5);
}

// Тест 3: Удаление элементов
TEST(DoublyListTest, PopFrontBack)
{
    MemoryResource mem(1024);
    DoublyLinkedList<int> list(&mem);

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_front();
    list.pop_back();

    auto it = list.begin();
    ASSERT_NE(it, list.end());
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(it, list.end());
}

// Тест 4: Проверка пустоты
TEST(DoublyListTest, EmptyCheck)
{
    MemoryResource mem(1024);
    DoublyLinkedList<int> list(&mem);

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);

    list.push_back(123);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 1);
}

// Тест 5: Очистка списка
TEST(DoublyListTest, ClearRemovesAll)
{
    MemoryResource mem(1024);
    DoublyLinkedList<int> list(&mem);

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(list.size(), 3);
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

// Тест 6: Перемещение списка
TEST(DoublyListTest, MoveSemantics)
{
    MemoryResource mem(1024);
    DoublyLinkedList<int> list1(&mem);

    list1.push_back(100);
    list1.push_back(200);

    DoublyLinkedList<int> list2 = std::move(list1);

    EXPECT_TRUE(list1.empty());
    EXPECT_EQ(list1.size(), 0);

    EXPECT_FALSE(list2.empty());
    EXPECT_EQ(list2.size(), 2);

    std::vector<int> v;
    for (auto x : list2)
        v.push_back(x);

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 100);
    EXPECT_EQ(v[1], 200);
}

// Тест 7: Сложный тип данных
TEST(DoublyListTest, ComplexType)
{
    struct Person
    {
        int id;
        std::string name;

        Person(int i = 0, std::string n = "") : id(i), name(std::move(n)) {}
        bool operator==(const Person &other) const
        {
            return id == other.id && name == other.name;
        }
    };

    MemoryResource mem(2048);
    DoublyLinkedList<Person> list(&mem);

    list.push_back(Person(1, "Alice"));
    list.push_back(Person(2, "Bob"));
    list.push_front(Person(0, "Admin"));

    EXPECT_EQ(list.size(), 3);

    auto it = list.begin();
    EXPECT_EQ(it->id, 0);
    EXPECT_EQ(it->name, "Admin");

    ++it;
    EXPECT_EQ(it->id, 1);
    EXPECT_EQ(it->name, "Alice");
}

// Тест 8: Const итераторы
TEST(DoublyListTest, ConstIterators)
{
    MemoryResource mem(1024);
    DoublyLinkedList<int> list(&mem);

    list.push_back(42);
    list.push_back(99);

    const auto &const_list = list;
    
    std::vector<int> v;
    for (auto it = const_list.cbegin(); it != const_list.cend(); ++it)
    {
        v.push_back(*it);
    }

    ASSERT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 42);
    EXPECT_EQ(v[1], 99);
}

// Тест 9: MemoryResource исчерпание памяти
TEST(MemoryResourceTest, OutOfMemory)
{
    MemoryResource mem(64); // Очень маленький буфер

    DoublyLinkedList<int> list(&mem);

    // Первое выделение должно работать
    list.push_back(1);

    // Второе может не сработать (зависит от размера Node)
    try
    {
        list.push_back(2);
        // Если дошли сюда, значит памяти хватило
        SUCCEED();
    }
    catch (const std::bad_alloc &)
    {
        // Ожидаемое исключение при нехватке памяти
        SUCCEED();
    }
    catch (...)
    {
        FAIL() << "Unexpected exception type";
    }
}

// Тест 10: Повторное использование памяти
TEST(MemoryResourceTest, MemoryReuse)
{
    MemoryResource mem(256);
    DoublyLinkedList<int> list(&mem);

    // Заполняем память
    for (int i = 0; i < 10; ++i)
    {
        list.push_back(i);
    }

    size_t size1 = list.size();

    // Освобождаем всю память
    list.clear();

    // Снова заполняем (память должна переиспользоваться)
    for (int i = 0; i < 10; ++i)
    {
        list.push_back(i * 10);
    }

    size_t size2 = list.size();

    EXPECT_EQ(size1, size2);
    EXPECT_EQ(size2, 10);
}

// Тест 11: Проверка равенства memory_resource
TEST(MemoryResourceTest, IsEqual)
{
    MemoryResource mem1(1024);
    MemoryResource mem2(1024);

    EXPECT_TRUE(mem1 == mem1);  // Должно быть равно самому себе
    EXPECT_FALSE(mem1 == mem2); // Разные объекты - не равны
}

// Тест 12: Обращение к пустому списку
TEST(DoublyListTest, EmptyListOperations)
{
    MemoryResource mem(1024);
    DoublyLinkedList<int> list(&mem);

    EXPECT_NO_THROW(list.pop_front()); // Должно ничего не делать
    EXPECT_NO_THROW(list.pop_back());  // Должно ничего не делать

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.begin(), list.end());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}