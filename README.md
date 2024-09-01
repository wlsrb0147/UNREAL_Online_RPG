## 목차

### 1. 프로젝트 설명
### 2. 기술서 & 깃허브 & 시연연상
### 3. 기능 시연 및 코드 위치
</br>

## 1. 프로젝트 설명

- 온라인 RPG를 제작하였습니다. 
    
- Listen 서버를 사용하며, 몬스터와 캐릭터의 행동 및 필드아이템 정보를 실시간 동기화하였습니다.
 
- 몬스터의 움직임은 Behavior tree, 캐릭터의 애니메이션은 State machine 을 사용하여 구현하였고, </br> 아이템의 정보는 Data table를 통해 관리하였습니다.
    
- 캐릭터의 정보는 Firebase에 저장합니다.
    
- 제가 담당한 작업은 캐릭터와 NPC/아이템 상호작용, 인벤토리와 아이템 상태관리입니다.
    

## 2. 기술서 & 시연영상

설명 PDF : https://drive.google.com/file/d/1VSAAhnuiWu1VIX4vAJXQsxYeGPjNsoON/view?usp=sharing
</br>
(목차 포함 10p)</br></br>
시연영상 : https://youtu.be/KNrG7cmZnLg 
</br></br>


## 3. 기능 시연 및 코드 위치

서버와 클라이언트 화면을 동시에 녹화했습니다.

Client는 Discord의 송출화면을 사용하여 약간의 딜레이가 존재합니다.

위쪽화면이 Server 이며, 아래쪽화면이 Client 입니다. 

메서드는 ()로 표기합니다.

아이템 기본 정보</br>
아이템 데이터 구조체 : Source/Online_RPG/ItemStruct</br>
아이템 구조 UObject : Source/Online_RPG/ItemBase</br>
아이템 데이터 테이블 : Content/Item/NewDataTable</br></br>

UI 구조</br>
![구조](https://github.com/user-attachments/assets/8f99a6ba-4dd2-4370-96f0-90c155079f54)
</br></br>


### 3-1 아이템 드랍 및 생성</br>
![2drop](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/1292436e-3145-4769-93d1-98b0d0d74f5d)
</br>
아이템 드래그 : Source/Online_RPG/ItemSlot</br>
아이템 드래그 감지 : Source/Online_RPG/InventoryMainMenu</br>
아이템 생성 : Source/Online_RPG/PlayerCharacter -> DropItem(~)</br>

### 3-2 아이템 획득 및 파괴</br>
![3get](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/2cb75afc-3a13-4d20-accf-798e004723e6)

아이템 감지 : Source/Online_RPG/PlayerCharacter => Tick() => CheckInteraction() </br>
아이템 상호작용 시작 : Source/Online_RPG/PlayerCharacter => Interact() (Bind Key F) </br>
아이템 상호작용 : Source/Online_RPG/PickUpItem => Interact() </br>
아이템 획득 및 파괴1 : Source/Online_RPG/PickUpItem => PickUpItem()</br>
아이템 획득 및 파괴2 : Source/Online_RPG/InventoryComponent => HandleAddItem()</br></br>

### 3-3 아이템 수량변화 온라인 동기화</br>
![4part](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/b47816b2-180e-44c4-b965-f34ccb6e6c79)

아이템 수량 변경 및 동기화 : Source/Online_RPG/PickUpItem => PickUpItem() => RPC_Set_Quantity() </br>

### 3-4 로그인/로그아웃시 데이터의 저장과 불러오기</br>
![5logout](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/af4d7e10-e4e8-4cdc-a371-ae2b2fa2aea5)

아이템 저장 : 아이템의 Key값과 Quantity값만 저장 하고, Key값으로 ItemBase를 탐색 후, Quantity의 개수를 인벤토리에 추가합니다.</br>
아이템 저장 기준 : 일정 시간 주기, 게임을 종료할 때, 퀘스트 진행도 또는 아이템 수량이 변경될 때마다 저장합니다. </br>
</br>
Firebase의 아이템 관련 데이터 불러오기 : Source/Online_RPG/Network_Manager_R => GetSpawnData_CallBack()</br>
아이템 인벤토리에 추가 : Source/Online_RPG/PlayerCharacter =>BeginPlay()</br>

아이템 정보 저장 : Source/Online_RPG/Network_Manager_R => UpdateSpawnData()</br>


