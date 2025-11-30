# Secret-Isolated Kyber for ARM TrustZone-A

TrustZone-A 기반 환경에서 **CRYSTALS-Kyber KEM**을  
**비밀 연산 / 공개 연산으로 분리(Split)** 해서 구현한 레포지토리입니다.

- **Secure World**: 비밀(seed, sk, noise) 및 *secret-dependent* 연산만 담당  
- **Normal World**: NTT, polynomial 연산, matrix A 생성 등 공개 연산 담당  
- 목표: **TCB(Trusted Computing Base) 최소화 + 성능 + 보안성** 동시에 달성

이 코드는 논문  
> *"A Lightweight Secret-Isolated Post-Quantum Cryptographic Architecture for ARM TrustZone-A"*  
에서 사용된 **Kyber Split 프로토타입 구현**을 기반으로 합니다.

---

## ✨ 특징 (Features)

- Kyber KEM(ML-KEM) **Split 아키텍처** 구현
  - 비밀 연산: Secure World (TEE)
  - 공개 연산: Normal World (REE / Linux user-space)
- **Secret-Isolation 보장**
  - `sk`, 비밀 seed, noise 등은 Secure World 메모리 밖으로 절대 노출되지 않음
- 기존 Kyber 레퍼런스 코드 기반 변형
  - NIST Kyber 레퍼런스 구조를 최대한 유지하되,  
    secret-dependent 부분만 TrustZone-A로 이동
- **TrustZone-A/OP-TEE** 환경에서 동작하도록 설계(QEMU 및 실제 보드 모두 지원 가능)

---

## 🏗 아키텍처 개요

### 1. Split 모델

**KeyGen**

- Normal World
  - 공개 seed 생성
  - matrix A 생성
  - NTT, polynomial 곱셈
  - public key `pk` 계산
- Secure World
  - `Secure_KeyGen_Secret(seed_secret)`
  - 비밀 키 `sk`, noise 샘플링
  - `sk`는 Secure RAM에만 저장

**Encaps**

- Normal World
  - Kyber Encapsulation 전체 수행 (NTT, noise, polynomial ops)
  - ciphertext `ct` 생성
- Secure World
  - 사용되지 않음 (PQC 구조상 Encaps는 비밀 의존성이 없음)

**Decaps**

- Normal World
  - `ct` 파싱, 압축 해제, NTT-ready polynomial 구성
- Secure World
  - `Secure_Decaps_Secret(ct)`
    - `sk`로 message `m` 복원
    - `KDF(m, ct)`로 shared secret 도출
    - masked/hashed shared secret만 REE로 반환  
  - `sk` 및 중간 비밀값은 Secure World 밖으로 노출되지 않음

---

## 📁 디렉터리 구조

```bash
├── README.md
├── .gitmodules
├── Kyber-split/
│   ├── normal
│   └── secure
├── OP-TEE/
│   ├── optee_client
│   └── optee_os
```

## OP_TEE 빌드 환경 준비
```
$ git clone https://github.com/OP-TEE/optee_os.git
$ # (toolchain, QEMU, build env 과정 진행)
```
