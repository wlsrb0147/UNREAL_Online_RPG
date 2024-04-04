## 목차

### 1. 프로젝트 설명
### 2. 기술서 & 깃허브 & 시연연상
### 3. 기능 시연
</br>

## 1. 프로젝트 설명

- 온라인 RPG를 제작하였습니다.
    
- Listen 서버를 사용하며, 몬스터와 캐릭터의 행동 및 필드아이템 정보를 실시간 동기화하였습니다.
 
- 몬스터의 움직임은 Behavior tree, 캐릭터의 애니메이션은 State machine 을 사용하여 구현하였고, </br> 아이템의 정보는 Data table를 통해 관리하였습니다.
    
- 캐릭터의 정보는 Firebase에 저장합니다.
    
- 제가 담당한 작업은 캐릭터와 NPC/아이템 상호작용, 인벤토리와 아이템 상태관리입니다.
    

## 2. 기술서 & 시연영상

시연영상 : https://youtu.be/KNrG7cmZnLg 
</br>
설명 PDF : https://drive.google.com/file/d/1VSAAhnuiWu1VIX4vAJXQsxYeGPjNsoON/view?usp=sharing
</br>
(목차 포함 10p)

## 3. 기능 시연

서버와 클라이언트 화면을 동시에 녹화했습니다.

Client는 Discord의 송출화면을 사용하여 약간의 딜레이가 존재합니다.

위쪽화면이 Server 이며, 아래쪽화면이 Client 입니다. 

### 3-1 캐릭터 움직임 동기화</br>
![1move](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/8f4b0207-cec8-4840-be18-d867c7b4b6de)

### 3-2 아이템 드랍 및 생성</br>
![2drop](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/1292436e-3145-4769-93d1-98b0d0d74f5d)

### 3-3 아이템 획득 및 파괴</br>
![3get](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/2cb75afc-3a13-4d20-accf-798e004723e6)

### 3-4 아이템 수량변화 동기화</br>
![4part](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/b47816b2-180e-44c4-b965-f34ccb6e6c79)

### 3-5 로그인/로그아웃시 데이터의 저장과 불러오기</br>
![5logout](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/af4d7e10-e4e8-4cdc-a371-ae2b2fa2aea5)

### 3-6 몬스터 움직임 동기화 및 몬스터 행동패턴</br>
![6monster](https://github.com/wlsrb0147/UNREAL_Online_RPG/assets/50743287/896286f0-f885-4d0c-b1ad-b1e63131e3aa)

