#include "Network.h"


const int INF = 1000000000;
Node::Node(int x_, int y_, int z_, int w_) {

	x = x_;
	y = y_;
	z = z_;
	w = w_;

	busy = false;

}

Node* Node::getNeighbor(int direction, Network* network) {

    if (direction == 0) {
        return network->getNode((x + 1) % network->getDimX(), y, z, w);
    }
    else if (direction == 1) {
        return network->getNode(x, (y + 1) % network->getDimY(), z, w);
    }
    else if (direction == 2) {
        return network->getNode(x, y, (z + 1) % network->getDimZ(), w);
    }
    
    return network->getNode(x, y, z, (w + 1) % network->getDimW());
    
}

Network::Network(int DimX_, int DimY_, int DimZ_, int DimW_) {

    DimX = DimX_, DimY = DimY_, DimZ = DimZ_, DimW = DimW_;

    // ������������� ���� �������� �����������
    nodes = new Node ***[DimX];
    for (int x = 0; x < DimX; x++) {
        nodes[x] = new Node **[DimY];
        for (int y = 0; y < DimY; y++) {
            nodes[x][y] = new Node * [DimZ];
            for (int z = 0; z < DimZ; z++) {
                nodes[x][y][z] = new Node[DimW];
                for (int w = 0; w < DimW; w++) {
                    nodes[x][y][z][w] = Node(x, y, z, w);
                }
            }
        }
    }

    //������ ���������� �����������
    bandwidth[0] = 3, bandwidth[1] = 3 , bandwidth[2] = 3, bandwidth[3] = 3;
}

Node* Network::getNode(int x, int y, int z, int w) {
    return &nodes[x][y][z][w];
}

std::vector<Node*> Network::routeSearch(Node* SourceAddress, Node* RecipientAddress) {
    
    if (SourceAddress == RecipientAddress) {
        std::cout << "�������� � ���������� ���������!" << std::endl;
        return std::vector<Node*>();
    }
        
    std::vector<Node*> MissedNodes;  // ������������ ����
    std::vector<Node*> NodesTraversed;  // ��������� ����
    std::unordered_map<Node*, int> ShortestPaths;  // ���������� ���� ��� ������� ����
    std::unordered_map<Node*, Node*> CameFrom;  // �������������� ���� ��� ������� ���� ��� �������������� ��������

    // ������� ������� ��������� ��������� ���� ��� ��������� ����
    ShortestPaths[SourceAddress] = 0; 
    //��������� ��� � ������ ������������ �����
    MissedNodes.push_back(SourceAddress);

    //���� ���� ������������ ����
    while (!MissedNodes.empty()) {
        //������� ��������� ����, ������� � �������
        Node* current = MissedNodes[0];
        for (Node* node : MissedNodes) {
            if (ShortestPaths[node] < ShortestPaths[current]) 
                current = node;           
        }

        // ���� �� �������� ��������� ����, ��������������� �������
        if (current == RecipientAddress) {
            std::vector<Node*> path;
            while (current != SourceAddress) {
                path.push_back(current);
                current = CameFrom[current];
            }
            path.push_back(SourceAddress);
            std::reverse(path.begin(), path.end());

            return path;
        }

        // ������� ������� ���� �� ������� ������������ �����
        MissedNodes.erase(std::remove(MissedNodes.begin(), MissedNodes.end(), current), MissedNodes.end());
        // � ��������� � ����������
        NodesTraversed.push_back(current);

        // ��������� ���� ������� �������� ����
        for (int direction = 0; direction < 4; direction++) {
            Node* neighbor = current->getNeighbor(direction, this);

            // ���� ����� ����� ��� ��� ����������, ���������� ���
            if (neighbor->isBusy() || std::find(NodesTraversed.begin(), NodesTraversed.end(), neighbor) != NodesTraversed.end()) {
                continue;
            }

            // ������������ ����� ��������� ���� �� ������
            int NewPath = ShortestPaths[current] + 1;

            // ���� ��� ������ ���, ����� �� ����� ����, ��� ����� ����� �������� ���� �� ����

            // ���� ��� ����� ����, ��������� ��� � ������ ������������ �����
            if (std::find(MissedNodes.begin(), MissedNodes.end(), neighbor) == MissedNodes.end()) {
                ShortestPaths[neighbor] = INF;
                MissedNodes.push_back(neighbor);
            }
            // ���� ����� ���� NewPath ������ �������� �� neighbor ��� �� ��� ������, ���������� ���� � ��� neighbor ������ current � �������������� ����
            if (NewPath < ShortestPaths[neighbor] || ShortestPaths.find(neighbor) == ShortestPaths.end()) {
                CameFrom[neighbor] = current;
                ShortestPaths[neighbor] = NewPath;
            }
        }
    }

    //std::cout << "���� �� " << SourceAddress << " � " << RecipientAddress <<  " �� ��� ������" << std::endl;
    return std::vector<Node*>();  
}

void pressAnyKeyToContinue() {
    std::cout << "������� ����� ������� ��� �������� � ���������� ����..." << std::endl;
    std::cin.clear(); // ������ �����
    std::cin.ignore(9999, '\n');
    std::cin.get();  // ���� ����� ������������
}

void Network::sendMessage(Network netw, Node* SourceAddress, int MessageLength, Node* RecipientAddress) {

    std::vector<Node*> AdressSourse;
    std::vector<int> LengthMessage;
    std::vector<Node*> AddressRecipient;

    AdressSourse.push_back(SourceAddress);
    LengthMessage.push_back(MessageLength);
    AddressRecipient.push_back(RecipientAddress);

    // ��������, ��� ��������� ����
    if (MessageLength == 0) {
        std::cout << "����� ��������� �������" << std::endl;
        return;
    }

    // ������� ���������� ���� ����� ������������ � �����������

    std::vector<std::vector<Node*>> path;

    if (routeSearch(SourceAddress, RecipientAddress) == std::vector<Node*>()) {
        std::cout << "���� �� ��� ������!" << std::endl;
        return;
    }

    path.push_back(routeSearch(SourceAddress, RecipientAddress));
   
    for (auto& BusyNodes : path[0])
        BusyNodes->setBusy(true);
      
    std::vector<std::unordered_map<Node*, int>>progress;
    progress.resize(1);
    
    progress[0][path[0][1]] = 0; // ������ ������� ����� ��� ������� ����

    bool flag = false; // ����, ������� ������ true, ����� �� ���������� ������ ���� �����
    int currentStep = 1; // ������� ��� �������

    //�������� ����� ��������
    while (!flag) {

        int CountTask=0;
        std::cout << "������� ���-�� (�� 0 �� 10) ����� ������� �� ���������: ";
        std::cin >> CountTask;

        for (int i = 0; i < CountTask; i++) {

            std::cout << "������� ����� ���������, ����� ��������� � ����� ���������� � ������� xxxx-y-xxxx:" << std::endl;

            std::string str1 = correctInput1();
            int length = correctInput2();
            std::string str2 = correctInput1();

            Node* sourse = netw.getNode(str1[0] - '0', str1[1] - '0', str1[2] - '0', str1[3] - '0');  // ���� (1,1,0,0)
            Node* recipient = netw.getNode(str2[0] - '0', str2[1] - '0', str2[2] - '0', str2[3] - '0');

            if (routeSearch(sourse, recipient)== std::vector<Node*>()) {
                std::cout << "���� �� ��� ������!" << std::endl;
                continue;
            }
            AdressSourse.push_back(sourse);
            LengthMessage.push_back(length);
            AddressRecipient.push_back(recipient);

            path.push_back(routeSearch(AdressSourse.back(), AddressRecipient.back()));
            
            for (auto& BusyNodes : path.back())
                BusyNodes->setBusy(true);                     

            progress.resize(progress.size()+1);
            
            progress[progress.size()-1][path[progress.size() - 1][1]] = 0;
            
        }

        std::cout << "��� " << currentStep << ": " << std::endl;
        
        for (int i = 0; i < progress.size(); i++) {

            for (auto& entry : progress[i]) {

                auto it = std::find(path[i].begin(), path[i].end(), entry.first);
                int index = std::distance(path[i].begin(), it);
                Node* CameFrom = path[i][index - 1];
                Node* ToNode = path[i][index];

                int ConnectionDirection = 0;
                if (CameFrom->getY() != ToNode->getY())
                    ConnectionDirection = 1;
                else if (CameFrom->getZ() != ToNode->getZ())
                    ConnectionDirection = 2;
                else if (CameFrom->getW() != ToNode->getW())
                    ConnectionDirection = 3;

                //�������� ����������� ��������� ���������� ������ � ������������ � ���������� ������������
                entry.second = std::min((entry.second + bandwidth[ConnectionDirection]), LengthMessage[i]);

                //������� � ������� �� ������ ���� � ������ ��� ������� �����, ������� ����� ����� ���� ���� ���� ��������
                std::cout << "   (" << CameFrom->getX() << "," << CameFrom->getY() << "," << CameFrom->getZ() << "," << CameFrom->getW() << ")";
                std::cout << " -> (" << ToNode->getX() << "," << ToNode->getY() << "," << ToNode->getZ() << "," << ToNode->getW() << ")";
                std::cout << " (�������� " << entry.second << " �� " << LengthMessage[i] << ")" << std::endl;

            }

            std::vector<std::unordered_map<Node*, int>> progressCopy = progress;

            // ��������� ����� ����� �� ����� �����
            for (auto& entry : progressCopy[i]) {

                auto it = std::find(path[i].begin(), path[i].end(), entry.first);
                int index = std::distance(path[i].begin(), it);

                if (index != path[i].size() - 1 && progress[i].find(path[i][index + 1]) == progress[i].end())
                    progress[i][path[i][index + 1]] = 0;

            }

            progressCopy = progress;

            // �� ����, ����� ������� ���� �������� ���������� ������, ������ ����� ���������, �� ������������� �����
            for (auto& entry : progressCopy[i]) {
                if (entry.second == LengthMessage[i]) {
                    auto it = std::find(path[i].begin(), path[i].end(), entry.first);
                    if (std::distance(path[i].begin(), it) == path[i].size() - 1) {
                       
                        for (auto& BusyNodes : path[i])
                            BusyNodes->setBusy(false);

                        progress.erase(progress.begin() + i);
                        path.erase(path.begin() + i);
                        LengthMessage.erase(LengthMessage.begin() + i);                    
                        AdressSourse.erase(AdressSourse.begin() + i);                       
                        AddressRecipient.erase(AddressRecipient.begin() + i);
                        i--;
                        break;
                    }
                    else {
                        progress[i].erase(entry.first);
                        entry.first->setBusy(false);
                    }
                        
                        


                }
            }
           
            if (path.size() == 0) {
                std::cout << "��� ��������� ���� ��������!" << std::endl;
                flag = true;
                break;
            }

        }
        
        // ����������� ���
        currentStep++;

        pressAnyKeyToContinue();
               
    }

    std::cout << "��������� �������� �� " << currentStep-1 << " �����." << std::endl;
}



