#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>


using namespace std;

int main()
{
	SetConsoleCP(1251); // Возможность читать и вносить через cin кирилицу
	SetConsoleOutputCP(1251);

	WSAData wsData; // Инициализация WinSock
	int checkStatus = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (checkStatus != 0)
	{
		cout << "Error WinSock version initializaion #" << WSAGetLastError() << endl;
		return 1;
	}
	else
	{
		cout << "WinSock successfully initialized" << endl;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0); // Создание client socket
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "Error initialization socket # " << WSAGetLastError() << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "Server socket successfully initialized" << endl;
	}

	in_addr ip_to_num; // Перевод IP в поток байтов
	checkStatus = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (checkStatus <= 0)
	{
		cout << "Error in IP translation to special numeric format" << endl;
		return 1;
	}

	sockaddr_in serverInfo; // Создание структуры для сервера
	ZeroMemory(&serverInfo, sizeof(serverInfo));
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr = ip_to_num;
	serverInfo.sin_port = htons(55555);

	checkStatus = connect(clientSocket, (sockaddr*)&serverInfo, sizeof(serverInfo));
	if (checkStatus != 0)
	{
		cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "Connection established SUCCESSFULLY." << endl;
	}

	string /*userName, login, password, */message;
	vector<char> buffer(1024);
	short count = 0, packet_size = 0;
	char open;

	cout << "Добро пожаловать в чат.\nВыберите,что вы хотите сделать:" << endl;
	cout << "1.[Создать аккаунт] 2.[Войти в аккаунт] 3.[Отобразить всех пользователей] 4.[Выйти из чата]" << endl;

	fgets(buffer.data(), buffer.size(), stdin);
	packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
	open = buffer[0];

	cout << endl;

	while (open != '4')
	{
		switch (open)
		{
		case '1':
		{
			cout << "Введите имя: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			cout << "Введите логин: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ожидает 1 или 0
			while (buffer[0] == '1')
			{
				cout << "Такой логин уже существует. Пожалуйста, придумайте другой логин: ";
				fgets(buffer.data(), buffer.size(), stdin);
				packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
				packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ожидает 1 или 0
			}
			cout << "Введите пароль: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			cout << endl;

			cout << "Аккаунт успешно создан\n" << endl;
			++count;

			break;
		}
		case '2':
		{
			cout << "Введите логин: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			cout << "Введите пароль: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			cout << endl;
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ожидает 0 или 1
			if (buffer[0] == '0')
			{
				char choiceInsideCase2;
				cout << "Выберите действие:" << endl << "1.[Написать сообщение] 2.[Показать сообщение] 3.[Сменить пароль] 4.[Выйти из аккаунта]" << endl;
				fgets(buffer.data(), buffer.size(), stdin);
				packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
				choiceInsideCase2 = buffer[0];
				cout << endl;

				while (choiceInsideCase2 != '4')
				{
					switch (choiceInsideCase2)
					{
					case '1':
					{
						char choiceInsideCase2InsideCase1;
						cout << "Кому вы хотите написать сообщение:\n1.[Определенному пользователю] 2.[Групповое сообщение] 3.[Вернутся назад]" << endl;
						fgets(buffer.data(), buffer.size(), stdin);
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						choiceInsideCase2InsideCase1 = buffer[0];

						while (choiceInsideCase2InsideCase1 != '3')
						{
							switch (choiceInsideCase2InsideCase1)
							{
							case '1': // сообщение пользователю
							{
								cout << "Пользователи:" << endl;
								short i = 0;
								while (i != count - 1)
								{
									packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
									cout << buffer.data() << endl;
									++i;
								}

								cout << endl;

								cout << "Введите логин пользователя кому хотите написать сообщение: ";
								fgets(buffer.data(), buffer.size(), stdin);
								packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
								packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ожидает 0 или 1
								if (buffer[0] == '0')
								{
									cout << "Введите сообщение: ";
									getline(cin, message);
									while (message.empty())
									{
										cout << "Сообщение не отправлено(отсутсвует текст). Введите сообщение: ";
										getline(cin, message);
									}
									buffer.clear();
									buffer.resize(1024);
									for (int i = 0; i < message.size(); ++i)
									{
										buffer[i] = message[i];
									}
									packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
									cout << "Сообщение успешно отправлено!" << endl;
									message.clear();
								}
								else
								{
									cout << "Неправильный логин" << endl;
									break;
								}

								break;
							}
							case '2': // групповой чат
							{
								cout << "Групповой чат ещё не реализован. Сообщения можно отправлять только на сервер. Для выхода введите \'q\'" << endl;
								while (true)
								{
									fgets(buffer.data(), buffer.size(), stdin);
									packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
									if (buffer[0] == 'q')
									{
										break;
									}
									packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
									for (int i = 0; i < 1024; ++i)
									{
										if (buffer[i] == '\n')
										{
											break;
										}
										else
										{
											message += buffer[i];
										}
									}
									std::cout << "Server: " << message << std::endl;
									message.clear();
								}

								break;
							}
							default:
							{
								cout << "Неверное значение\n" << endl;

								break;
							}
							}
							cout << endl;
							cout << "Кому вы хотите написать сообщение:\n1.[Определенному пользователю] 2.[Групповое сообщение] 3.[Вернутся назад]" << endl;
							fgets(buffer.data(), buffer.size(), stdin);
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							choiceInsideCase2InsideCase1 = buffer[0];
						};

						break;
					}
					case '2': // показать сообщения
					{
						packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); 
						int msgCount = (int)buffer[0];
						short i = 0;
						while (i != msgCount)
						{
							packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
							cout << buffer.data() << endl;
							++i;
						}
						cout << "Нажмите любую клавишу, чтобы продолжить..." << endl;
						system("pause>0"); // Ставит программу на паузу, чтобы пользователь прочитал сообщение
						buffer.clear();
						buffer.resize(1024);

						break;
					}
					case '3': //Смена пароля на новый
					{
						cout << "Введите старый пароль: ";
						fgets(buffer.data(), buffer.size(), stdin);
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ожидает 0 или 1
						while (buffer[0] == '1')
						{
							cout << "Пароль неверный\nВведите старый пароль: ";
							fgets(buffer.data(), buffer.size(), stdin);
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ожидает 0 или 1
						}
						buffer.clear(); // think about it
						buffer.resize(1024);
						cout << "Введите новый пароль: ";
						fgets(buffer.data(), buffer.size(), stdin);
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ожидает 0 или 1
						while (buffer[0] == '1')
						{
							cout << "Это ваш старый пароль! Придумайте другой пароль: ";
							fgets(buffer.data(), buffer.size(), stdin);
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ожидает 0 или 1
						}
						cout << "Пароль успешно изменен!\n" << endl;

						break;
					}
					default:
					{
						cout << "Неверное значение\n" << endl;

						break;
					}
					}

					cout << "Выберите действие:\n1.[Написать сообщение] 2.[Показать сообщение] 3.[Сменить пароль] 4.[Выйти из аккаунта]" << endl;
					fgets(buffer.data(), buffer.size(), stdin);
					packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
					choiceInsideCase2 = buffer[0];

					cout << endl;
				};

				cout << endl;

				break;
			}
			else
			{
				cout << "Неверный логин или пароль\n" << endl;
				break;
			}
			break;
		}
		case '3':
		{
			short i = 0;
			while (i != count)
			{
				packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
				cout << buffer.data() << endl;
				++i;
			}
			cout << endl;
			buffer.clear();
			buffer.resize(1024);

			break;
		}
		default:
		{
			cout << "Неверное значение\n" << endl;

			break;
		};
		}
		cout << "1.[Создать аккаунт] 2.[Войти в аккаунт] 3.[Отобразить всех пользователей] 4.[Выйти из чата]" << endl;

		fgets(buffer.data(), buffer.size(), stdin);
		packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
		open = buffer[0];
		cout << endl;
	}

	closesocket(clientSocket);
	WSACleanup();

	return 0;
}