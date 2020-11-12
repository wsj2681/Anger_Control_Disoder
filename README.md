## 게임
* 제목 : Anger_Control_Disorder
```
DirectX12로 구현한 3D 1:1 PvP 복싱 게임입니다.
```
## platform
* Visual Studio 2017
```
visula Studio Release Version : 15.9.29
SDK version : 10.0.18362.0
플랫폼 도구 집합 : v141
```
## SDK
* DirectX12
```
3D게임 개발을 위한 엔진입니다.
```
* FMOD
```
사운드 출력을 위한 라이브러리 입니다.
```
* FBX SDK
```
FBX파일을 로드하기 위한 라이브러리 입니다.
```
## 코드 규칙
* 기본은 이 코딩 표준을 사용합니다.
```
https://docs.popekim.com/ko/coding-standards/cpp
```
### 변경, 수정사항
* 9 클래스 멤버 변수명은 앞에 m을 붙인다
```
-> 필요한 경우에 m을 붙인다 구분이 필요하다면 변수명을 다르게한다.
-> 멤버 변수는 this->로 구분한다.
```
* 15
```
헤더파일을 인클루드 할 때 framework.h 는 cpp파일의 맨 최상단에 위치한다.
그 후에 해당 클래스의 헤더파일 등을 사용한다.
```
* 31 클래스 안에서 멤버 변수와 메서드의 등장 순서는 다음을 따른다.
```
1. public: 생성자
2. private: 맴버 변수
3. protected :멤버 변수
4. public: 맴버 함수
```

* ASSERT(함수 인자 != nullptr, "인자값이 nullptr 입니다.")
```
받아온 함수 인자가 내가 원하는 값이 아니라면 ASSERT 경고를 호출합니다.
```
* HR( hResult 반환 함수 );
```
함수를 실행할 때 그 값이 HRESULT를 반환한다면 이 매크로를 이용하여 사용합니다.
혹시라도 디버깅이 필요할 땐 상황에 따라 변수를 사용하여 함수를 호출합니다.
```
