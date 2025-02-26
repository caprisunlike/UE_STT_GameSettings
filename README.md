# 캡스톤디자인 프로젝트 STT 파트 2024-2


## 순서
1. '1'키를 누르면 녹음이 시작되고 누르는 것을 멈추면 녹음이 종료된 후 wav파일이 생성됨
2. 녹음이 종료되면 Check for New File 함수에서 MyAudioWatcher BP의 Speech to Text 이벤트와 Update Result 이벤트가 순차적으로 실행됨
3. Speech to Text 이벤트에서 wav파일이 존재하는 것이 확인되면 ProcessAudioFile 함수(cpp에 구현)가 실행되며, STT 결과값을 받아오고 녹음이 다시 시작될 경우 기존의 wav파일로 인한 에러를 방지하기 위해 STT API를 거친 wav파일을 삭제함
4. 파일이 존재하지 않으면 0.2초의 시간 후 다시 파일의 존재를 확인함
5. STT 결과값에 따라 수행해야할 명령이 0~7로 구분되며 UpdateResult에서 결과값이 업데이트됨을 확인함(only 업데이트 확인용)
6. 명령 구분 후 값이 올바르게 저장되면 Setting Change 이벤트가 실행되고 명령을 수행함
![그림2](https://github.com/user-attachments/assets/4b212b51-ee7d-4fd9-bbdf-3e6b3a2337f0)


## Asset
![만들어야하는거](https://github.com/user-attachments/assets/911847c1-47fb-480b-a7a9-2cf624c1676a)

## MyAudioWatcher Blueprint
- 컴포넌트 및 변수

  ![audiowatchervalue](https://github.com/user-attachments/assets/3dd34334-acdd-4de9-b986-91edad36285d)
- 이벤트 그래프
  - 메인 이벤트
    ![stt메인함수](https://github.com/user-attachments/assets/7d4f591a-b95d-43d1-9ee0-2242e3ad725f)
  - STT 결과 업데이트
    ![결과업데이트](https://github.com/user-attachments/assets/95b27930-55dc-4af9-8b8a-58059d5e4cd5)
  - 실행할 함수 결정
    ![그림1](https://github.com/user-attachments/assets/06f5fb2a-8fcd-4dc9-9a61-82d1fccc846d)
- 함수
  - 나이아가라 이펙트 on/off
    ![showrain](https://github.com/user-attachments/assets/0728c6c3-3f4c-4f3d-a792-7cfe19a7a314)
  - 마우스 감도 up/down
    ![changesens](https://github.com/user-attachments/assets/d991bfe4-0039-44c6-81ae-68feffc7f2bd)
  - 화면 밝기 up/down
    ![changebright](https://github.com/user-attachments/assets/4a441054-8cc2-44c9-97c9-2f34f24fa801)
  - 소리 up/down
    ![changevolume](https://github.com/user-attachments/assets/906cb25b-350c-45a4-add5-1adee118d700)


## Character Blueprint
- 컴포넌트 및 변수

  ![캐릭터변수설정](https://github.com/user-attachments/assets/6aedca81-9a74-4c29-9f95-d51bccb2191b)
- 이벤트 그래프
  - 키보드 입력
    ![캐릭터](https://github.com/user-attachments/assets/c62fc120-ca96-4267-abfd-8301d2eef101)
  - 마우스 감도 설정
    ![캐릭터마우스감도](https://github.com/user-attachments/assets/5063da9f-2cf4-4f63-aff5-02dbcbb4906e)
- 함수
  - 새로운 오디오 파일이 만들어졌는지 확인
    ![newaudiofile확인](https://github.com/user-attachments/assets/96d5120f-c7ca-4806-8d60-bd882315ef4f)

## For loop with delay 매크로
![forloopdelay매크로](https://github.com/user-attachments/assets/0eb160e7-a866-4c33-9c84-fb6c6e9676c8)
