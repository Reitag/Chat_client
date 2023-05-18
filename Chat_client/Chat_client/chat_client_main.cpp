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
	SetConsoleCP(1251); // ����������� ������ � ������� ����� cin ��������
	SetConsoleOutputCP(1251);

	WSAData wsData; // ������������� WinSock
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

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0); // �������� client socket
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

	in_addr ip_to_num; // ������� IP � ����� ������
	checkStatus = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (checkStatus <= 0)
	{
		cout << "Error in IP translation to special numeric format" << endl;
		return 1;
	}

	sockaddr_in serverInfo; // �������� ��������� ��� �������
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

	cout << "����� ���������� � ���.\n��������,��� �� ������ �������:" << endl;
	cout << "1.[������� �������] 2.[����� � �������] 3.[���������� ���� �������������] 4.[����� �� ����]" << endl;

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
			cout << "������� ���: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			cout << "������� �����: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ������� 1 ��� 0
			while (buffer[0] == '1')
			{
				cout << "����� ����� ��� ����������. ����������, ���������� ������ �����: ";
				fgets(buffer.data(), buffer.size(), stdin);
				packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
				packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ������� 1 ��� 0
			}
			cout << "������� ������: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			cout << endl;

			cout << "������� ������� ������\n" << endl;
			++count;

			break;
		}
		case '2':
		{
			cout << "������� �����: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			cout << "������� ������: ";
			fgets(buffer.data(), buffer.size(), stdin);
			packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
			cout << endl;
			packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ������� 0 ��� 1
			if (buffer[0] == '0')
			{
				char choiceInsideCase2;
				cout << "�������� ��������:" << endl << "1.[�������� ���������] 2.[�������� ���������] 3.[������� ������] 4.[����� �� ��������]" << endl;
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
						cout << "���� �� ������ �������� ���������:\n1.[������������� ������������] 2.[��������� ���������] 3.[�������� �����]" << endl;
						fgets(buffer.data(), buffer.size(), stdin);
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						choiceInsideCase2InsideCase1 = buffer[0];

						while (choiceInsideCase2InsideCase1 != '3')
						{
							switch (choiceInsideCase2InsideCase1)
							{
							case '1': // ��������� ������������
							{
								cout << "������������:" << endl;
								short i = 0;
								while (i != count - 1)
								{
									packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL);
									cout << buffer.data() << endl;
									++i;
								}

								cout << endl;

								cout << "������� ����� ������������ ���� ������ �������� ���������: ";
								fgets(buffer.data(), buffer.size(), stdin);
								packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
								packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ������� 0 ��� 1
								if (buffer[0] == '0')
								{
									cout << "������� ���������: ";
									getline(cin, message);
									while (message.empty())
									{
										cout << "��������� �� ����������(���������� �����). ������� ���������: ";
										getline(cin, message);
									}
									buffer.clear();
									buffer.resize(1024);
									for (int i = 0; i < message.size(); ++i)
									{
										buffer[i] = message[i];
									}
									packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
									cout << "��������� ������� ����������!" << endl;
									message.clear();
								}
								else
								{
									cout << "������������ �����" << endl;
									break;
								}

								break;
							}
							case '2': // ��������� ���
							{
								cout << "��������� ��� ��� �� ����������. ��������� ����� ���������� ������ �� ������. ��� ������ ������� \'q\'" << endl;
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
								cout << "�������� ��������\n" << endl;

								break;
							}
							}
							cout << endl;
							cout << "���� �� ������ �������� ���������:\n1.[������������� ������������] 2.[��������� ���������] 3.[�������� �����]" << endl;
							fgets(buffer.data(), buffer.size(), stdin);
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							choiceInsideCase2InsideCase1 = buffer[0];
						};

						break;
					}
					case '2': // �������� ���������
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
						cout << "������� ����� �������, ����� ����������..." << endl;
						system("pause>0"); // ������ ��������� �� �����, ����� ������������ �������� ���������
						buffer.clear();
						buffer.resize(1024);

						break;
					}
					case '3': //����� ������ �� �����
					{
						cout << "������� ������ ������: ";
						fgets(buffer.data(), buffer.size(), stdin);
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ������� 0 ��� 1
						while (buffer[0] == '1')
						{
							cout << "������ ��������\n������� ������ ������: ";
							fgets(buffer.data(), buffer.size(), stdin);
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ������� 0 ��� 1
						}
						buffer.clear(); // think about it
						buffer.resize(1024);
						cout << "������� ����� ������: ";
						fgets(buffer.data(), buffer.size(), stdin);
						packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
						packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ������� 0 ��� 1
						while (buffer[0] == '1')
						{
							cout << "��� ��� ������ ������! ���������� ������ ������: ";
							fgets(buffer.data(), buffer.size(), stdin);
							packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
							packet_size = recv(clientSocket, buffer.data(), buffer.size(), MSG_WAITALL); // ������� 0 ��� 1
						}
						cout << "������ ������� �������!\n" << endl;

						break;
					}
					default:
					{
						cout << "�������� ��������\n" << endl;

						break;
					}
					}

					cout << "�������� ��������:\n1.[�������� ���������] 2.[�������� ���������] 3.[������� ������] 4.[����� �� ��������]" << endl;
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
				cout << "�������� ����� ��� ������\n" << endl;
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
			cout << "�������� ��������\n" << endl;

			break;
		};
		}
		cout << "1.[������� �������] 2.[����� � �������] 3.[���������� ���� �������������] 4.[����� �� ����]" << endl;

		fgets(buffer.data(), buffer.size(), stdin);
		packet_size = send(clientSocket, buffer.data(), buffer.size(), 0);
		open = buffer[0];
		cout << endl;
	}

	closesocket(clientSocket);
	WSACleanup();

	return 0;
}