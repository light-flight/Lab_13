# Lab_13
laboratory work

### ЛАБОРАТОРНАЯ РАБОТА 13.
### УКАЗАТЕЛИ. ДИНАМИЧЕСКИЕ СТРУКТУРЫ ДАННЫХ. ОБРАБОТКА БИНАРНЫХ ДЕРЕВЬЕВ

#### Задание
Дан текст, хранящийся в файле. Текст состоит из слов, разделенных пробелами – длина слова не более 20 букв. Решить простейшую задачу создания конкорданса. Реализовать функцию создания, упорядоченного (в алфавитном порядке) бинарного дерева, где в узлах хранятся слова и количество их появления в указанном тексте. Реализовать различные способы обхода дерева (в глубину, в ширину, в обратном порядке и т.д.) для вывода элементов на экран. Реализовать задачу, приведенную в варианте, в виде отдельной рекурсивной функции.

##### 27 вариант
Посчитать количество листьев в дереве, имеющих заданную длину.

**Выполнил Юрьев Георгий, группа ПМ-13**
******************************************************************************************

ОС: Windows 7 home premium SP1.<br>
Среда разработки: Microsoft Visual Studio Community 2013. Версия 12.0.31101.00 Update 4.

******************************************************************************************

### Проблемы:

Создаваемые программой файлы корректно отображаются (и латинские, и кириллические символы) при просмотре в стандартном блокноте windows. Чтобы просмотреть файлы в других текстовых редакторах, например, notepad++, мне приходилось пересохранять результирующие текстовые файлы с кодировкой UTF-8, используя Visual Studio.

******************************************************************************************

### Отклонения от задания:

Когда я понял, что могу обрабатывать таким образом объемные тексты, то было решено расширить условия задачи, чтобы беспрепятственно анализировать реальные литературные произведения.

##### Добавления:
+ слова разделяются не только пробелами, но и другими знаками препинания и прочими символами.
+ максимальный размер слов - 30 букв, т.к. встречались сложные слова по 25 букв и т.д.

##### Изменения:
+ весь вывод осуществляется в, создаваемые в ходе работы программы,
текстовые файлы, т.к. при работе с большими текстами неудобно
просматривать результат в окне консоли.

******************************************************************************************

### Описание работы программы:
- Перед запуском программы необходимо ввести выбранный для анализа текст в файл "text.txt".
- В ходе работы программы необходимо ввести длину слова (число от 1 до 30). Согласно заданию, программа выведет в файл "words_by_length.txt" все слова соответствующие заданой длине.
- Также, будет создан файл "concordance.txt", который будет содержать алфавитный перечень всех слов с указанием их повторений. Если слово повторяется единожды, то число повторений для него не выводится.
- Вывод слов осуществляется в три колонки для удобства. Все рабочие текстовые файлы располагаются в корневой папке относительно файла исполняемого кода.
