/*!
@file DrawComponents.cpp
@brief �`��R���|�[�l���g����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"
#include <pix.h>


namespace basecross {


	//������ʃV�F�[�_�[
	IMPLEMENT_DX12SHADER(VSShadowmap, App::GetApp()->m_wstrRelativeShadersPath + L"VSShadowmap.cso")


	IMPLEMENT_DX12SHADER(VSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCDirect.cso")
	IMPLEMENT_DX12SHADER(PSPCDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCDirect.cso")

	IMPLEMENT_DX12SHADER(VSPTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTDirect.cso")
	IMPLEMENT_DX12SHADER(PSPTDirect, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTDirect.cso")

	IMPLEMENT_DX12SHADER(VSPCSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCSprite.cso")
	IMPLEMENT_DX12SHADER(PSPCSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCSprite.cso")

	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTSprite.cso")

	IMPLEMENT_DX12SHADER(VSPCTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPCTSprite, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCTSprite.cso")


	IMPLEMENT_DX12SHADER(VSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCStatic.cso")
	IMPLEMENT_DX12SHADER(PSPCStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCStatic.cso")

	IMPLEMENT_DX12SHADER(VSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPTStatic.cso")

	IMPLEMENT_DX12SHADER(VSPCTInstance, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTInstance.cso")
	IMPLEMENT_DX12SHADER(VSPCTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPCTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPCTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPCTStatic.cso")


	IMPLEMENT_DX12SHADER(VSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNTStatic, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStatic.cso")

	IMPLEMENT_DX12SHADER(VSPNTStaticShadow, App::GetApp()->m_wstrRelativeShadersPath + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX12SHADER(PSPNTStaticShadow, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX12SHADER(PSPNTStaticShadow2, App::GetApp()->m_wstrRelativeShadersPath + L"PSPNTStaticShadow2.cso")



	//--------------------------------------------------------------------------------------
	//	struct DrawComponent::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DrawComponent::Impl {
		BlendState m_BlendState;
		DepthStencilState m_DepthStencilState;
		RasterizerState m_RasterizerState;
		SamplerState m_SamplerState;
		bsm::Mat4x4 m_MeshToTransformMatrix;
		Impl() :
			m_MeshToTransformMatrix() {}
	};

	//--------------------------------------------------------------------------------------
	///	�`��R���|�[�l���g�̐e�N���X����
	//--------------------------------------------------------------------------------------
	DrawComponent::DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	DrawComponent::~DrawComponent() {}


	BlendState DrawComponent::GetBlendState() const {
		return pImpl->m_BlendState;
	}
	DepthStencilState DrawComponent::GetDepthStencilState() const {
		return pImpl->m_DepthStencilState;
	}
	RasterizerState DrawComponent::GetRasterizerState() const {
		return pImpl->m_RasterizerState;
	}
	SamplerState DrawComponent::GetSamplerState() const {
		return pImpl->m_SamplerState;
	}


	void DrawComponent::SetBlendState(const BlendState state) {
		pImpl->m_BlendState = state;
	}
	void DrawComponent::SetDepthStencilState(const DepthStencilState state) {
		pImpl->m_DepthStencilState = state;

	}
	void DrawComponent::SetRasterizerState(const RasterizerState state) {
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetSamplerState(const SamplerState state) {
		pImpl->m_SamplerState = state;
	}

	const bsm::Mat4x4& DrawComponent::GetMeshToTransformMatrix() const {
		return pImpl->m_MeshToTransformMatrix;
	}
	void DrawComponent::SetMeshToTransformMatrix(const bsm::Mat4x4& Mat) {
		pImpl->m_MeshToTransformMatrix = Mat;
	}

	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl {
		static float m_LightHeight;	//���C�g�̍����i���������̒l�Ŋ|����j
		static float m_LightNear;	//���C�g��Near
		static float m_LightFar;		//���C�g��Far
		static float m_ViewWidth;
		static float m_ViewHeight;

		weak_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X

												///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		// �R���X�^���g�o�b�t�@
		struct ShadowConstantBuffer
		{
			bsm::Mat4x4 mWorld;
			bsm::Mat4x4 mView;
			bsm::Mat4x4 mProj;
			bsm::Vec4 Bones[3 * 72];	//Bone�p
			ShadowConstantBuffer() {
				memset(this, 0, sizeof(ShadowConstantBuffer));
			};
		};
		ShadowConstantBuffer m_ShadowConstantBuffer;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		//�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer();
		///�`�揈��
		void DrawObject();

		Impl()
		{
			try {
				///���[�g�V�O�l�`���쐬
				CreateRootSignature();
				///�f�X�N�v���^�q�[�v�쐬
				CreateDescriptorHeap();
				///�R���X�^���g�o�b�t�@�쐬
				CreateConstantBuffer();
				///�p�C�v���C���X�e�[�g�쐬
				CreatePipelineState();
				///�R�}���h���X�g�쐬
				CreateCommandList();
				//�R���X�^���g�o�b�t�@�X�V
				UpdateConstantBuffer();
			}
			catch (...) {
				throw;

			}

		}
		~Impl() {}
	};

	float Shadowmap::Impl::m_LightHeight(20.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(200.0f);
	float Shadowmap::Impl::m_ViewWidth(32.0f);
	float Shadowmap::Impl::m_ViewHeight(32.0f);

	///���[�g�V�O�l�`���쐬
	void Shadowmap::Impl::CreateRootSignature() {
		//�R���X�^���g�o�b�t�@�t���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void Shadowmap::Impl::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize
			= Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void Shadowmap::Impl::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(ShadowConstantBuffer) + 255) & ~255;
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"Shadowmap::Impl::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"Shadowmap::Impl::CreateConstantBuffer()"
		);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void Shadowmap::Impl::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState = PipelineState::CreateShadowmap3D<VertexPositionNormalTexture, VSShadowmap>(m_RootSignature, PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void Shadowmap::Impl::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}


	void Shadowmap::Impl::UpdateConstantBuffer() {
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_ShadowConstantBuffer),
			sizeof(m_ShadowConstantBuffer));
	}

	///�`�揈��
	void Shadowmap::Impl::DrawObject() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto ShMesh = m_MeshResource.lock();
		if (!ShMesh) {
			return;
		}
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);
		//���b�V�����X�V����Ă���΃��\�[�X�X�V
		ShMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		//���[�g�V�O�l�`���̃Z�b�g
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		//�f�X�N�v���^�q�[�v�̃Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GPU�f�X�N�v���^�q�[�v�n���h���̃Z�b�g
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}

		auto ShadowMapDimension = Dev->GetShadowMapRenderTarget()->GetShadowMapDimension();


		D3D12_VIEWPORT Viewport = {};
		Viewport.Width = static_cast<float>(ShadowMapDimension);
		Viewport.Height = static_cast<float>(ShadowMapDimension);
		Viewport.MaxDepth = 1.0f;

		D3D12_RECT ScissorRect = {};

		ScissorRect.right = static_cast<LONG>(ShadowMapDimension);
		ScissorRect.bottom = static_cast<LONG>(ShadowMapDimension);


		m_CommandList->RSSetViewports(1, &Viewport);
		m_CommandList->RSSetScissorRects(1, &ScissorRect);

		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		auto SMRenderTarget = Dev->GetShadowMapRenderTarget();
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = SMRenderTarget->GetDsvHandle();
		//�擾�����n���h�����Z�b�g
		m_CommandList->OMSetRenderTargets(0, nullptr, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());
		//�C���f�b�N�X�o�b�t�@���Z�b�g
		m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		//�C���f�b�N�X�`��
		m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());


	}






	//--------------------------------------------------------------------------------------
	//	�V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
	//--------------------------------------------------------------------------------------
	Shadowmap::Shadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	Shadowmap::~Shadowmap() {}

	float Shadowmap::GetLightHeight() { return Impl::m_LightHeight; }
	float Shadowmap::GetLightNear() { return  Impl::m_LightNear; }
	float Shadowmap::GetLightFar() { return  Impl::m_LightFar; }
	float Shadowmap::GetViewWidth() { return  Impl::m_ViewWidth; }
	float Shadowmap::GetViewHeight() { return  Impl::m_ViewHeight; }

	void Shadowmap::SetLightHeight(float f) { Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f) { Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f) { Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f) { Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f) { Impl::m_ViewHeight = f; }
	void Shadowmap::SetViewSize(float f) { Impl::m_ViewWidth = Impl::m_ViewHeight = f; }

	shared_ptr<MeshResource> Shadowmap::GetMeshResource(bool ExceptionActive) const {
		if (!pImpl->m_MeshResource.expired()) {
			return pImpl->m_MeshResource.lock();
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_MeshResource.expired())",
					L"ShadowmapComp::GetMeshResource()"
				);
			}
		}
		return nullptr;
	}


	void Shadowmap::SetMeshResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMeshResource()"
				);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Shadowmap::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr) {
		pImpl->m_MeshResource = MeshResourcePtr;
	}

	void Shadowmap::OnDraw() {
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage) {
			return;
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();

		//�s��̒�`
		bsm::Mat4x4 World, LightView, LightProj;
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//���[���h�s��̌���
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//�r���[�s��̌���
		auto StageView = PtrStage->GetView();
		//���C�g�̎擾
		auto StageLight = PtrStage->GetLight();
		bsm::Vec3 LightDir = -1.0 * StageLight->GetTargetLight().m_Directional;
		bsm::Vec3 LightAt = StageView->GetTargetCamera()->GetAt();
		bsm::Vec3 LightEye = LightAt + (LightDir * GetLightHeight());
		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView.LookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
		LightProj.OrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

		pImpl->m_ShadowConstantBuffer.mWorld = bsm::Mat4x4EX::Transpose(World);
		pImpl->m_ShadowConstantBuffer.mView = bsm::Mat4x4EX::Transpose(LightView);
		pImpl->m_ShadowConstantBuffer.mProj = bsm::Mat4x4EX::Transpose(LightProj);

		pImpl->UpdateConstantBuffer();
		pImpl->DrawObject();
	}

	//--------------------------------------------------------------------------------------
	// static�n�R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	struct StaticConstantBuffer
	{
		bsm::Mat4x4 World;
		bsm::Mat4x4 View;
		bsm::Mat4x4 Projection;
		bsm::Col4 Emissive;
		bsm::Col4 Diffuse;
		StaticConstantBuffer() {
			memset(this, 0, sizeof(StaticConstantBuffer));
			Diffuse = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};


	//--------------------------------------------------------------------------------------
	//ParticleDraw�����̂��߂̍\����
	//--------------------------------------------------------------------------------------
	struct DrawParticleSprite {
		//�p�[�e�B�N���̃J�����܂ł̋���
		float m_ToCaneraLength;
		//���[���h�s��
		bsm::Mat4x4 m_WorldMatrix;
		//�e�N�X�`��
		shared_ptr<TextureResource> m_TextureRes;
		DrawParticleSprite() :
			m_ToCaneraLength(0)
		{}
	};

	//--------------------------------------------------------------------------------------
	//ParticleDraw�C���X�^���X�`��̂��߂̍\����
	//--------------------------------------------------------------------------------------
	struct InstanceDrawStr {
		size_t Start;
		size_t Count;
		shared_ptr<TextureResource> Tex;
		InstanceDrawStr(size_t s, size_t c, shared_ptr<TextureResource> t) :
			Start(s), Count(c), Tex(t) {}
	};


	//--------------------------------------------------------------------------------------
	//	struct PCTParticleDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PCTParticleDraw::Impl {
		shared_ptr<MeshResource> m_MeshResource;	///<���b�V�����\�[�X
		shared_ptr<MeshResource> m_InstanceMatrixMesh;	///<�}�g���N�X���b�V��
		vector<DrawParticleSprite> m_DrawParticleSpriteVec;
		const size_t m_MaxInstance;				///<�C���X�^���X�ő�l
		ComPtr<ID3D11Buffer> m_MatrixBuffer;	///<�s��p�̒��_�o�b�t�@

		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;


		Impl(size_t MaxInstance) :
			m_MaxInstance(MaxInstance)
		{}
		~Impl() {}

		///�e�������֐�
		//���_�o�b�t�@�̍쐬
		void CreateParticleBuffers();
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView(const shared_ptr<TextureResource>& TexRes);
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr);
		///�`�揈��
		void DrawObject();



	};

	void PCTParticleDraw::Impl::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			bsm::Vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//���_�z��
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),  col,bsm::Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), col, bsm::Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0),  col,bsm::Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0),  col, bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);

			//�C���X�^���X�s��o�b�t�@�̍쐬
			//Max�l�ō쐬����
			vector<bsm::Mat4x4> matrices(m_MaxInstance, bsm::Mat4x4());
			//�C���X�^���X�`��p�̍s��̃��b�V���i���e�ύX�ł���j
			m_InstanceMatrixMesh = MeshResource::CreateMeshResource(matrices, true);
		}
		catch (...) {
			throw;
		}
	}

	///���[�g�V�O�l�`���쐬
	void PCTParticleDraw::Impl::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void PCTParticleDraw::Impl::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
		//�T���v���[�f�X�N�v���^�q�[�v
		m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�T���v���[�쐬
	void PCTParticleDraw::Impl::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PCTParticleDraw::Impl::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(StaticConstantBuffer) + 255) & ~255;

		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"PCTParticleDraw::Impl::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"PCTParticleDraw::Impl::CreateConstantBuffer()"
		);
	}
	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void PCTParticleDraw::Impl::CreateShaderResourceView(const shared_ptr<TextureResource>& TexRes) {
		auto Dev = App::GetApp()->GetDeviceResources();



		m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			TexRes->GetTexture().Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST));


		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = TexRes->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = TexRes->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			TexRes->GetTexture().Get(),
			&srvDesc,
			Handle);
		m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			TexRes->GetTexture().Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	}
	///�p�C�v���C���X�e�[�g�쐬
	void PCTParticleDraw::Impl::CreatePipelineState() {
		//�p�C�v���C���X�e�[�g�̍쐬
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		PipelineState::CreateDefault3D<VertexPositionColorTextureMatrix, VSPCTInstance, PSPCTStatic>(m_RootSignature, PineLineDesc);
		PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;

		D3D12_BLEND_DESC blend_desc;
		D3D12_RENDER_TARGET_BLEND_DESC Target;
		ZeroMemory(&blend_desc, sizeof(blend_desc));
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = false;
		ZeroMemory(&Target, sizeof(Target));
		Target.BlendEnable = true;
		Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		Target.BlendOp = D3D12_BLEND_OP_ADD;
		Target.SrcBlendAlpha = D3D12_BLEND_ONE;
		Target.DestBlendAlpha = D3D12_BLEND_ZERO;
		Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
			blend_desc.RenderTarget[i] = Target;
		}
		PineLineDesc.BlendState = blend_desc;
		m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void PCTParticleDraw::Impl::CreateCommandList() {
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	void PCTParticleDraw::Impl::UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr) {
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;

		//�J�����𓾂�
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.Transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();

		//�R���X�^���g�o�b�t�@�̏���
		StaticConstantBuffer cb;
		cb.World = bsm::Mat4x4();	//���[���h�s��̓_�~�[
		cb.View = ViewMat;
		cb.Projection = ProjMat;
		//�f�B�t���[�Y
		cb.Diffuse = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		cb.Emissive = bsm::Col4(0, 0, 0, 0);
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&cb),
			sizeof(StaticConstantBuffer));
	}


	///�`�揈��
	void PCTParticleDraw::Impl::DrawObject() {

		//�J�����ʒu�Ń\�[�g
		auto func = [](DrawParticleSprite& Left, DrawParticleSprite& Right)->bool {
			return (Left.m_ToCaneraLength > Right.m_ToCaneraLength);
		};
		std::sort(m_DrawParticleSpriteVec.begin(), m_DrawParticleSpriteVec.end(), func);
		//�s��̕ύX
		vector<bsm::Mat4x4> MatVec;
		for (auto& v : m_DrawParticleSpriteVec) {
			bsm::Mat4x4 World = v.m_WorldMatrix;
			//�]�u����
			World.Transpose();
			MatVec.push_back(World);
		}
		//���b�V���̒��_�̕ύX
		m_InstanceMatrixMesh->UpdateVirtex(MatVec);



		//�擪�̃e�N�X�`���𓾂�
		auto  NowTexPtr = m_DrawParticleSpriteVec[0].m_TextureRes;

		vector<InstanceDrawStr> InstancVec;
		size_t NowStartIndex = 0;
		size_t NowDrawCount = 0;

		shared_ptr<TextureResource> NowTexRes = m_DrawParticleSpriteVec[0].m_TextureRes;
		for (size_t i = 0; i < m_DrawParticleSpriteVec.size(); i++) {
			if (m_DrawParticleSpriteVec[i].m_TextureRes != NowTexRes) {
				InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes));
				NowStartIndex = i;
				NowDrawCount = 0;
				NowTexRes = m_DrawParticleSpriteVec[i].m_TextureRes;
			}
			else {
				NowDrawCount++;
			}
		}
		InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes));


		CommandList::Reset(m_PipelineState, m_CommandList);

		m_MeshResource->UpdateResources<VertexPositionColorTexture>(m_CommandList);
		m_InstanceMatrixMesh->UpdateResources<bsm::Mat4x4>(m_CommandList);

		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());


		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&m_MeshResource->GetIndexBufferView());
		//�C���X�^���X�`��́A���_�o�b�t�@�𕡐��o�^����
		const D3D12_VERTEX_BUFFER_VIEW Buf[2] = {
			m_MeshResource->GetVertexBufferView(),
			m_InstanceMatrixMesh->GetVertexBufferView()
		};
		m_CommandList->IASetVertexBuffers(0, 2, Buf);

		for (auto& v : InstancVec) {
			v.Tex->UpdateResources(m_CommandList);
			CreateShaderResourceView(v.Tex);
			//�`��
			m_CommandList->DrawIndexedInstanced(m_MeshResource->GetNumIndicis(), v.Count, 0, 0, v.Start);
		}
		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());


		//�p�[�e�B�N���̔z��̃N���A
		m_DrawParticleSpriteVec.clear();
	}






	//--------------------------------------------------------------------------------------
	///	PCTParticle�`��R���|�[�l���g(�p�[�e�B�N���`��)
	//--------------------------------------------------------------------------------------

	PCTParticleDraw::PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance):
		DrawComponent(GameObjectPtr),
		pImpl(new Impl(MaxInstance))
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	PCTParticleDraw::~PCTParticleDraw() {}

	void PCTParticleDraw::AddParticle(float ToCaneraLength, const bsm::Mat4x4& WorldMatrix, const shared_ptr<TextureResource>& TextureRes) {
		DrawParticleSprite Item;
		Item.m_ToCaneraLength = ToCaneraLength;
		Item.m_WorldMatrix = WorldMatrix;
		Item.m_TextureRes = TextureRes;
		pImpl->m_DrawParticleSpriteVec.push_back(Item);


	}
	void PCTParticleDraw::OnCreate(){
		pImpl->CreateParticleBuffers();
		///�e�������֐��Ăяo��
		///���[�g�V�O�l�`���쐬
		pImpl->CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		pImpl->CreateDescriptorHeap();
		///�T���v���[�쐬
		pImpl->CreateSampler();
		///�R���X�^���g�o�b�t�@�쐬
		pImpl->CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		pImpl->CreatePipelineState();
		///�R�}���h���X�g�쐬
		pImpl->CreateCommandList();
		//�R���X�^���g�o�b�t�@�̍X�V
//		pImpl->UpdateConstantBuffer(GetGameObject());

	}
	void PCTParticleDraw::OnDraw(){
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		if (pImpl->m_DrawParticleSpriteVec.size() <= 0) {
			return;
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject());
		pImpl->DrawObject();
	}


	//--------------------------------------------------------------------------------------
	//	struct TextureDrawInterface::Impl;
	//--------------------------------------------------------------------------------------
	struct TextureDrawInterface::Impl {
		//�e�N�X�`��
		weak_ptr<TextureResource> m_TextureResource;
		//���b�v�T���v���[���ǂ���
		bool m_WrapSampler;
		Impl() :
			m_WrapSampler(false)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	�e�N�X�`���`��C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	TextureDrawInterface::TextureDrawInterface() :
		pImpl(new Impl())
	{}

	TextureDrawInterface::~TextureDrawInterface() {}

	void TextureDrawInterface::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
	}

	void TextureDrawInterface::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
	}

	shared_ptr<TextureResource> TextureDrawInterface::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = pImpl->m_TextureResource.lock();
		if (!shptr) {
			return nullptr;
		}
		return shptr;
	}

	bool TextureDrawInterface::GetWrapSampler() const {
		return pImpl->m_WrapSampler;

	}
	void TextureDrawInterface::SetWrapSampler(bool b) {
		pImpl->m_WrapSampler = b;
	}



	//--------------------------------------------------------------------------------------
	//	struct SpriteBaseDraw::Impl;
	//	�p�r: SpriteBaseDraw�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SpriteBaseDraw::Impl {
		//���b�V��
		shared_ptr<MeshResource> m_SpriteMesh;
		//�G�~�b�V�u�F
		bsm::Col4 m_Emissive;
		//�f�t���[�Y�F
		bsm::Col4 m_Diffuse;
		Impl() :
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	class SpriteBaseDraw : public DrawComponent;
	//--------------------------------------------------------------------------------------
	SpriteBaseDraw::SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	SpriteBaseDraw::~SpriteBaseDraw() {}


	shared_ptr<MeshResource> SpriteBaseDraw::GetMeshResource() const {
		return pImpl->m_SpriteMesh;
	}
	void SpriteBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SpriteMesh = MeshRes;
	}


	bsm::Col4 SpriteBaseDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void SpriteBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_Emissive = col;
	}

	bsm::Col4 SpriteBaseDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void SpriteBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_Diffuse = col;
	}

	//--------------------------------------------------------------------------------------
	//	�X�v���C�g�p�R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	struct SpriteConstantBuffer
	{
		bsm::Mat4x4 World;
		bsm::Col4 Emissive;
		bsm::Col4 Diffuse;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
			Diffuse = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};

	//--------------------------------------------------------------------------------------
	//	struct PCSpriteDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCSpriteDraw::Impl {
		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		//�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		bool m_Trace;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Col4& Emissive, const bsm::Col4& Diffuse);
		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& MeshRes,bool Trace);
		Impl():
			m_Trace(false)
		{}
	};

	///���[�g�V�O�l�`���쐬
	void PCSpriteDraw::Impl::CreateRootSignature() {
		//�R���X�^���g�o�b�t�@�t���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void PCSpriteDraw::Impl::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize
			= Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PCSpriteDraw::Impl::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(SpriteConstantBuffer) + 255) & ~255;
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"PCSpriteDraw::Impl::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"PCSpriteDraw::Impl::CreateConstantBuffer()"
		);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void PCSpriteDraw::Impl::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_RootSignature, PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void PCSpriteDraw::Impl::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	void PCSpriteDraw::Impl::UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse) {
		//�s��̒�`
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		bsm::Vec2 Scale, Pos, Pivot;
		Scale.x = PtrTrans->GetScale().x;
		Scale.y = PtrTrans->GetScale().y;
		Pos.x = PtrTrans->GetPosition().x;
		Pos.y = PtrTrans->GetPosition().y;
		Pivot.x = PtrTrans->GetPivot().x;
		Pivot.y = PtrTrans->GetPivot().y;
		bsm::Vec3 Rot = PtrTrans->GetRotation();

		//�s��̒�`
		bsm::Mat4x4 World, Proj;
		//���[���h�s��̌���
		World.AffineTransformation2D(
			Scale,			//�X�P�[�����O
			Pivot,		//��]�̒��S�i�d�S�j
			Rot.z,			//��]�p�x(Z����])
			Pos				//�ʒu
		);
		//�ˉe�s��̌���
		GameObjectPtr->OnGet2DDrawProjMatrix(Proj);
		//�s��̍���
		World *= Proj;
		//�R���X�^���g�o�b�t�@
		SpriteConstantBuffer cb;
		//�G�~�b�V�u
		cb.Emissive = Emissive;
		//�f�t�B�[�Y�͂��ׂĒʂ�
		cb.Diffuse = Diffuse;
		//�s��̐ݒ�
		cb.World = World;
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&cb),
			sizeof(SpriteConstantBuffer));
	}


	///�`�揈��
	void PCSpriteDraw::Impl::DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace) {
		auto Dev = App::GetApp()->GetDeviceResources();
		if (m_Trace != Trace) {
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
			m_PipelineState = PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_RootSignature, PineLineDesc);
			//�����̏ꍇ�̓u�����h�X�e�[�g�����ւ�
			if (Trace) {
				D3D12_BLEND_DESC blend_desc;
				D3D12_RENDER_TARGET_BLEND_DESC Target;
				ZeroMemory(&blend_desc, sizeof(blend_desc));
				blend_desc.AlphaToCoverageEnable = false;
				blend_desc.IndependentBlendEnable = false;
				ZeroMemory(&Target, sizeof(Target));
				Target.BlendEnable = true;
				Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
				Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				Target.BlendOp = D3D12_BLEND_OP_ADD;
				Target.SrcBlendAlpha = D3D12_BLEND_ONE;
				Target.DestBlendAlpha = D3D12_BLEND_ZERO;
				Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
				Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
					blend_desc.RenderTarget[i] = Target;
				}
				PineLineDesc.BlendState = blend_desc;
				m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
				m_Trace = Trace;
			}
		}
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);
		//���b�V�����X�V����Ă���΃��\�[�X�X�V
		MeshRes->UpdateResources<VertexPositionColor>(m_CommandList);
		//���[�g�V�O�l�`���̃Z�b�g
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		//�f�X�N�v���^�q�[�v�̃Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GPU�f�X�N�v���^�q�[�v�n���h���̃Z�b�g
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		//�r���[�|�[�g�̃Z�b�g
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		//�V�U�[��`�i�N���b�v��`�j�̃Z�b�g
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Dev->GetRtvHandle();
		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Dev->GetDsvHandle();
		//�擾�����n���h�����Z�b�g
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		//�`����@�̃Z�b�g�i�O�p�`���X�g�j
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//���_�o�b�t�@�̃Z�b�g
		m_CommandList->IASetVertexBuffers(0, 1, &MeshRes->GetVertexBufferView());
		//�C���f�b�N�X�o�b�t�@���Z�b�g
		m_CommandList->IASetIndexBuffer(&MeshRes->GetIndexBufferView());
		//�C���f�b�N�X�`��
		m_CommandList->DrawIndexedInstanced(MeshRes->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());


	}






	//--------------------------------------------------------------------------------------
	///	PCSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		try {
			//�p�C�v���C���X�e�[�g���f�t�H���g��2D
			SetBlendState(BlendState::Opaque);
			SetDepthStencilState(DepthStencilState::None);
			SetRasterizerState(RasterizerState::CullBack);
			SetSamplerState(SamplerState::LinearClamp);
			//���b�V���̍쐬�i�ύX�ł���j
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	PCSpriteDraw::~PCSpriteDraw() {}

	void PCSpriteDraw::CreateMesh(vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) {
		try {
			//���b�V���̍쐬�i�ύX�ł���j
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	void PCSpriteDraw::UpdateVertices(const vector<VertexPositionColor>& Vertices) {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!SpriteMesh)",
				L"PCSpriteDraw::UpdateVertices()"
			);
		}
		//���b�V���̒��_�̕ύX
		SpriteMesh->UpdateVirtex(Vertices);
	}
	void PCSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionColor> Vertices = {
				{ VertexPositionColor(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
				{ VertexPositionColor(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬�i�ύX�ł���j
			SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		///���[�g�V�O�l�`���쐬
		pImpl->CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		pImpl->CreateDescriptorHeap();
		///�R���X�^���g�o�b�t�@�쐬
		pImpl->CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		pImpl->CreatePipelineState();
		///�R�}���h���X�g�쐬
		pImpl->CreateCommandList();
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse());
	}
	void PCSpriteDraw::OnDraw() {
		//���b�V�����Ȃ���Ε`�悵�Ȃ�
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!SpriteMesh)",
				L"PCSpriteDraw::OnDraw()"
			);
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse());
		pImpl->DrawObject(SpriteMesh, GetGameObject()->IsAlphaActive());
	}



	//--------------------------------------------------------------------------------------
	//	struct PTSpriteDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PTSpriteDraw::Impl {
		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		bool m_Trace;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		///�e�������֐�
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		bool m_Wrap;
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView(const shared_ptr<TextureResource>& TextureRes);
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		///�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Col4& Emissive, const bsm::Col4& Diffuse);
		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace,
			const shared_ptr<TextureResource>& TextureRes, bool WrapSampler);
		Impl():
			m_Trace(false),
			m_Wrap(false)
		{}
	};

	///���[�g�V�O�l�`���쐬
	void PTSpriteDraw::Impl::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void PTSpriteDraw::Impl::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
		//�T���v���[�f�X�N�v���^�q�[�v
		m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�T���v���[�쐬
	void PTSpriteDraw::Impl::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///�V�F�[�_�[���\�[�X�r���[�쐬
	void PTSpriteDraw::Impl::CreateShaderResourceView(const shared_ptr<TextureResource>& TextureRes) {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = TextureRes->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = TextureRes->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			TextureRes->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PTSpriteDraw::Impl::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(SpriteConstantBuffer) + 255) & ~255;

		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"PTSpriteDraw::Impl::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"PTSpriteDraw::Impl::CreateConstantBuffer()"
		);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void PTSpriteDraw::Impl::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(m_RootSignature, PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void PTSpriteDraw::Impl::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///�R���X�^���g�o�b�t�@�X�V
	void PTSpriteDraw::Impl::UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse) {
		//�s��̒�`
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		bsm::Vec2 Scale, Pos, Pivot;
		Scale.x = PtrTrans->GetScale().x;
		Scale.y = PtrTrans->GetScale().y;
		Pos.x = PtrTrans->GetPosition().x;
		Pos.y = PtrTrans->GetPosition().y;
		Pivot.x = PtrTrans->GetPivot().x;
		Pivot.y = PtrTrans->GetPivot().y;
		bsm::Vec3 Rot = PtrTrans->GetRotation();

		//�s��̒�`
		bsm::Mat4x4 World, Proj;
		//���[���h�s��̌���
		World.AffineTransformation2D(
			Scale,			//�X�P�[�����O
			Pivot,		//��]�̒��S�i�d�S�j
			Rot.z,			//��]�p�x(Z����])
			Pos				//�ʒu
		);
		//�ˉe�s��̌���
		GameObjectPtr->OnGet2DDrawProjMatrix(Proj);
		//�s��̍���
		World *= Proj;
		SpriteConstantBuffer cb;
		//�G�~�b�V�u
		cb.Emissive = Emissive;
		//�f�t�B�[�Y�͂��ׂĒʂ�
		cb.Diffuse = Diffuse;
		//�s��̐ݒ�
		cb.World = World;
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&cb),
			sizeof(SpriteConstantBuffer));

	}
	///�`�揈��
	void PTSpriteDraw::Impl::DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace,
		const shared_ptr<TextureResource>& TextureRes, bool WrapSampler) {
		if (m_Wrap != WrapSampler) {
			auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			if (WrapSampler) {
				//���b�s���O�T���v���[
				DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
			}
			else {
				DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
			}
			m_Wrap = WrapSampler;
		}
		if (m_Trace != Trace) {
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
			m_PipelineState = PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(m_RootSignature, PineLineDesc);
			//�����̏ꍇ�̓u�����h�X�e�[�g�����ւ�
			if (Trace) {
				D3D12_BLEND_DESC blend_desc;
				D3D12_RENDER_TARGET_BLEND_DESC Target;
				ZeroMemory(&blend_desc, sizeof(blend_desc));
				blend_desc.AlphaToCoverageEnable = false;
				blend_desc.IndependentBlendEnable = false;
				ZeroMemory(&Target, sizeof(Target));
				Target.BlendEnable = true;
				Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
				Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				Target.BlendOp = D3D12_BLEND_OP_ADD;
				Target.SrcBlendAlpha = D3D12_BLEND_ONE;
				Target.DestBlendAlpha = D3D12_BLEND_ZERO;
				Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
				Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
					blend_desc.RenderTarget[i] = Target;
				}
				PineLineDesc.BlendState = blend_desc;
				m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
				m_Trace = Trace;
			}
		}
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

		MeshRes->UpdateResources<VertexPositionTexture>(m_CommandList);
		TextureRes->UpdateResources(m_CommandList);

		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&MeshRes->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &MeshRes->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(MeshRes->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}



	//--------------------------------------------------------------------------------------
	///	PTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		try {
			//�p�C�v���C���X�e�[�g���f�t�H���g��2D
			SetBlendState(BlendState::Opaque);
			SetDepthStencilState(DepthStencilState::None);
			SetRasterizerState(RasterizerState::CullBack);
			SetSamplerState(SamplerState::LinearClamp);
			//���b�V���̍쐬�i�ύX�ł���j
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	PTSpriteDraw::~PTSpriteDraw() {}

	void PTSpriteDraw::CreateMesh(vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//���b�V���̍쐬�i�ύX�ł���j
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	void PTSpriteDraw::UpdateVertices(const vector<VertexPositionTexture>& Vertices) {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!SpriteMesh)",
				L"PTSpriteDraw::UpdateVertices()"
			);
		}
		//���b�V���̒��_�̕ύX
		SpriteMesh->UpdateVirtex(Vertices);
	}

	void PTSpriteDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		TextureDrawInterface::SetTextureResource(TextureRes);
		pImpl->CreateShaderResourceView(TextureRes);
	}
	void PTSpriteDraw::SetTextureResource(const wstring& TextureKey) {
		TextureDrawInterface::SetTextureResource(TextureKey);
		pImpl->CreateShaderResourceView(GetTextureResource());
	}


	void PTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionTexture> Vertices = {
				{ VertexPositionTexture(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Vec2(0.0f, 0.0f)) },
				{ VertexPositionTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Vec2(1.0f, 0.0f)) },
				{ VertexPositionTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Vec2(0.0f, 1.0f)) },
				{ VertexPositionTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬�i�ύX�ł���j
			SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		///���[�g�V�O�l�`���쐬
		pImpl->CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		pImpl->CreateDescriptorHeap();
		///�T���v���[�쐬
		pImpl->CreateSampler();
		///�R���X�^���g�o�b�t�@�쐬
		pImpl->CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		pImpl->CreatePipelineState();
		///�R�}���h���X�g�쐬
		pImpl->CreateCommandList();
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse());

	}

	void PTSpriteDraw::OnDraw() {
		//���b�V�����Ȃ���Ε`�悵�Ȃ�
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!SpriteMesh)",
				L"PCSpriteDraw::OnDraw()"
			);
		}
		//�e�N�X�`�����Ȃ���Ε`�悵�Ȃ�
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse());
		pImpl->DrawObject(SpriteMesh, GetGameObject()->IsAlphaActive(),
			shTex, GetWrapSampler());
	}

	//--------------------------------------------------------------------------------------
	//	struct PCTSpriteDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCTSpriteDraw::Impl {
		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		bool m_Trace;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		///�e�������֐�
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		bool m_Wrap;
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView(const shared_ptr<TextureResource>& TextureRes);
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		///�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Col4& Emissive, const bsm::Col4& Diffuse);
		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace,
			const shared_ptr<TextureResource>& TextureRes, bool WrapSampler);
		Impl() :
			m_Trace(false),
			m_Wrap(false)
		{}
	};

	///���[�g�V�O�l�`���쐬
	void PCTSpriteDraw::Impl::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void PCTSpriteDraw::Impl::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
		//�T���v���[�f�X�N�v���^�q�[�v
		m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�T���v���[�쐬
	void PCTSpriteDraw::Impl::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///�V�F�[�_�[���\�[�X�r���[�쐬
	void PCTSpriteDraw::Impl::CreateShaderResourceView(const shared_ptr<TextureResource>& TextureRes) {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = TextureRes->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = TextureRes->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			TextureRes->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PCTSpriteDraw::Impl::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(SpriteConstantBuffer) + 255) & ~255;

		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"PCTSpriteDraw::Impl::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"PCTSpriteDraw::Impl::CreateConstantBuffer()"
		);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void PCTSpriteDraw::Impl::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault2D<VertexPositionColorTexture, VSPCTSprite, PSPCTSprite>(m_RootSignature, PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void PCTSpriteDraw::Impl::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///�R���X�^���g�o�b�t�@�X�V
	void PCTSpriteDraw::Impl::UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse) {
		//�s��̒�`
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		bsm::Vec2 Scale, Pos, Pivot;
		Scale.x = PtrTrans->GetScale().x;
		Scale.y = PtrTrans->GetScale().y;
		Pos.x = PtrTrans->GetPosition().x;
		Pos.y = PtrTrans->GetPosition().y;
		Pivot.x = PtrTrans->GetPivot().x;
		Pivot.y = PtrTrans->GetPivot().y;
		bsm::Vec3 Rot = PtrTrans->GetRotation();

		//�s��̒�`
		bsm::Mat4x4 World, Proj;
		//���[���h�s��̌���
		World.AffineTransformation2D(
			Scale,			//�X�P�[�����O
			Pivot,		//��]�̒��S�i�d�S�j
			Rot.z,			//��]�p�x(Z����])
			Pos				//�ʒu
		);
		//�ˉe�s��̌���
		GameObjectPtr->OnGet2DDrawProjMatrix(Proj);
		//�s��̍���
		World *= Proj;
		SpriteConstantBuffer cb;
		//�G�~�b�V�u
		cb.Emissive = Emissive;
		//�f�t�B�[�Y�͂��ׂĒʂ�
		cb.Diffuse = Diffuse;
		//�s��̐ݒ�
		cb.World = World;
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&cb),
			sizeof(SpriteConstantBuffer));

	}
	///�`�揈��
	void PCTSpriteDraw::Impl::DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace,
		const shared_ptr<TextureResource>& TextureRes, bool WrapSampler) {
		if (m_Wrap != WrapSampler) {
			auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			if (WrapSampler) {
				//���b�s���O�T���v���[
				DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
			}
			else {
				DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
			}
			m_Wrap = WrapSampler;
		}
		if (m_Trace != Trace) {
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
			m_PipelineState = PipelineState::CreateDefault2D<VertexPositionColorTexture, VSPCTSprite, PSPCTSprite>(m_RootSignature, PineLineDesc);
			//�����̏ꍇ�̓u�����h�X�e�[�g�����ւ�
			if (Trace) {
				D3D12_BLEND_DESC blend_desc;
				D3D12_RENDER_TARGET_BLEND_DESC Target;
				ZeroMemory(&blend_desc, sizeof(blend_desc));
				blend_desc.AlphaToCoverageEnable = false;
				blend_desc.IndependentBlendEnable = false;
				ZeroMemory(&Target, sizeof(Target));
				Target.BlendEnable = true;
				Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
				Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				Target.BlendOp = D3D12_BLEND_OP_ADD;
				Target.SrcBlendAlpha = D3D12_BLEND_ONE;
				Target.DestBlendAlpha = D3D12_BLEND_ZERO;
				Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
				Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
					blend_desc.RenderTarget[i] = Target;
				}
				PineLineDesc.BlendState = blend_desc;
				m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
				m_Trace = Trace;
			}
		}
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

		MeshRes->UpdateResources<VertexPositionColorTexture>(m_CommandList);
		TextureRes->UpdateResources(m_CommandList);

		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&MeshRes->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &MeshRes->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(MeshRes->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}




	//--------------------------------------------------------------------------------------
	///	PCTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		try {
			//�p�C�v���C���X�e�[�g���f�t�H���g��2D
			SetBlendState(BlendState::Opaque);
			SetDepthStencilState(DepthStencilState::None);
			SetRasterizerState(RasterizerState::CullBack);
			SetSamplerState(SamplerState::LinearClamp);
			//���b�V���̍쐬�i�ύX�ł���j
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}
	PCTSpriteDraw::~PCTSpriteDraw() {}

	void PCTSpriteDraw::CreateMesh(vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//���b�V���̍쐬�i�ύX�ł���j
			auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		catch (...) {
			throw;
		}
	}

	void PCTSpriteDraw::UpdateVertices(const vector<VertexPositionColorTexture>& Vertices) {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!SpriteMesh)",
				L"PTSpriteDraw::UpdateVertices()"
			);
		}
		//���b�V���̒��_�̕ύX
		SpriteMesh->UpdateVirtex(Vertices);
	}

	void PCTSpriteDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		TextureDrawInterface::SetTextureResource(TextureRes);
		pImpl->CreateShaderResourceView(TextureRes);
	}
	void PCTSpriteDraw::SetTextureResource(const wstring& TextureKey) {
		TextureDrawInterface::SetTextureResource(TextureKey);
		pImpl->CreateShaderResourceView(GetTextureResource());
	}


	void PCTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬�i�ύX�ł���j
			SpriteMesh = MeshResource::CreateMeshResource(vertices, indices, true);
			SetMeshResource(SpriteMesh);
		}
		///���[�g�V�O�l�`���쐬
		pImpl->CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		pImpl->CreateDescriptorHeap();
		///�T���v���[�쐬
		pImpl->CreateSampler();
		///�R���X�^���g�o�b�t�@�쐬
		pImpl->CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		pImpl->CreatePipelineState();
		///�R�}���h���X�g�쐬
		pImpl->CreateCommandList();
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse());

	}

	void PCTSpriteDraw::OnDraw() {
		//���b�V�����Ȃ���Ε`�悵�Ȃ�
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!SpriteMesh)",
				L"PCTSpriteDraw::OnDraw()"
			);
		}
		//�e�N�X�`�����Ȃ���Ε`�悵�Ȃ�
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse());
		pImpl->DrawObject(SpriteMesh, GetGameObject()->IsAlphaActive(),
			shTex, GetWrapSampler());
	}


	//--------------------------------------------------------------------------------------
	//	struct Base3DDraw::Impl;
	//	�p�r: Base3DDraw�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Base3DDraw::Impl {
		//�G�~�b�V�u�F
		bsm::Col4 m_Emissive;
		//�f�t���[�Y�F
		bsm::Col4 m_Diffuse;
		//�r���[�s��
		bsm::Mat4x4 m_ViewMatrix;
		//�ˉe�s��
		bsm::Mat4x4 m_ProjMatrix;
		Impl() :
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_ViewMatrix(),
			m_ProjMatrix()
		{}
	};

	//--------------------------------------------------------------------------------------
	///	3D�`��R���|�[�l���g�̐e(3D�`��̐e)
	//--------------------------------------------------------------------------------------
	Base3DDraw::Base3DDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	Base3DDraw::~Base3DDraw() {}
	bsm::Col4 Base3DDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void Base3DDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_Emissive = col;
	}
	bsm::Col4 Base3DDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void Base3DDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_Diffuse = col;
	}

	const bsm::Mat4x4& Base3DDraw::GetViewMatrix() const {
		return pImpl->m_ViewMatrix;
	}
	void Base3DDraw::SetViewMatrix(const bsm::Mat4x4& mat) {
		pImpl->m_ViewMatrix = mat;
	}
	const bsm::Mat4x4& Base3DDraw::GetProjMatrix() const {
		return pImpl->m_ProjMatrix;
	}
	void Base3DDraw::SetProjMatrix(const bsm::Mat4x4& mat) {
		pImpl->m_ProjMatrix = mat;
	}




	//--------------------------------------------------------------------------------------
	//	struct DynamicBaseDraw::Impl;
	//	�p�r: DynamicBaseDraw�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DynamicBaseDraw::Impl {
		//���b�V��
		shared_ptr<MeshResource> m_MeshResource;
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	Dynamic�`��R���|�[�l���g�̐e(���_��ύX�ł���3D�`��)
	//--------------------------------------------------------------------------------------
	DynamicBaseDraw::DynamicBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		Base3DDraw(GameObjectPtr),
		pImpl(new Impl())
	{}
	DynamicBaseDraw::~DynamicBaseDraw() {}
	shared_ptr<MeshResource> DynamicBaseDraw::GetMeshResource() const {
		return pImpl->m_MeshResource;
	}
	void DynamicBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}



	//--------------------------------------------------------------------------------------
	//	struct PCDynamicDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCDynamicDraw::Impl {
		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		//�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		bool m_Trace;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		///�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix);
		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace);
		Impl() :
			m_Trace(false)
		{}
	};


	///���[�g�V�O�l�`���쐬
	void PCDynamicDraw::Impl::CreateRootSignature() {
		//�R���X�^���g�o�b�t�@�t���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void PCDynamicDraw::Impl::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize
			= Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PCDynamicDraw::Impl::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(StaticConstantBuffer) + 255) & ~255;
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"PCDynamicDraw::Impl::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"PCDynamicDraw::Impl::CreateConstantBuffer()"
		);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void PCDynamicDraw::Impl::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault3D<VertexPositionColor, VSPCStatic, PSPCStatic>(m_RootSignature, PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void PCDynamicDraw::Impl::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///�R���X�^���g�o�b�t�@�X�V
	void PCDynamicDraw::Impl::UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix) {
		//�s��̒�`
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
		//�]�u����
		World.Transpose();
		//�J�����𓾂�
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.Transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();
		//�R���X�^���g�o�b�t�@�̏���
		StaticConstantBuffer cb;
		cb.World = World;
		cb.View = ViewMat;
		cb.Projection = ProjMat;
		//�G�~�b�V�u
		cb.Emissive = Emissive;
		//�f�t�B�[�Y
		cb.Diffuse = Diffuse;
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&cb),
			sizeof(StaticConstantBuffer));

	}

	///�`�揈��
	void PCDynamicDraw::Impl::DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace) {
		if (m_Trace != Trace) {
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
			m_PipelineState = PipelineState::CreateDefault3D<VertexPositionColor, VSPCStatic, PSPCStatic>(m_RootSignature, PineLineDesc);
			//�����̏ꍇ�̓u�����h�X�e�[�g�����ւ�
			if (Trace) {
				D3D12_BLEND_DESC blend_desc;
				D3D12_RENDER_TARGET_BLEND_DESC Target;
				ZeroMemory(&blend_desc, sizeof(blend_desc));
				blend_desc.AlphaToCoverageEnable = false;
				blend_desc.IndependentBlendEnable = false;
				ZeroMemory(&Target, sizeof(Target));
				Target.BlendEnable = true;
				Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
				Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				Target.BlendOp = D3D12_BLEND_OP_ADD;
				Target.SrcBlendAlpha = D3D12_BLEND_ONE;
				Target.DestBlendAlpha = D3D12_BLEND_ZERO;
				Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
				Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
					blend_desc.RenderTarget[i] = Target;
				}
				PineLineDesc.BlendState = blend_desc;
				m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
				m_Trace = Trace;
			}
		}
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

		MeshRes->UpdateResources<VertexPositionColor>(m_CommandList);

		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&MeshRes->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &MeshRes->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(MeshRes->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}




	//--------------------------------------------------------------------------------------
	///	PCDynamic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCDynamicDraw::PCDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DynamicBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCDynamicDraw::~PCDynamicDraw() {}

	void PCDynamicDraw::CreateMesh(vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) {
		try {
			//���b�V���̍쐬�i�ύX�ł���j
			auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(MeshRes);
		}
		catch (...) {
			throw;
		}
	}

	void PCDynamicDraw::UpdateVertices(const vector<VertexPositionColor>& Vertices) {
		auto Mesh = GetMeshResource();
		if (!Mesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!Mesh)",
				L"PCDynamicDraw::UpdateVertices()"
			);
		}
		//���b�V���̒��_�̕ύX
		Mesh->UpdateVirtex(Vertices);

	}
	void PCDynamicDraw::OnCreate() {
		///���[�g�V�O�l�`���쐬
		pImpl->CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		pImpl->CreateDescriptorHeap();
		///�R���X�^���g�o�b�t�@�쐬
		pImpl->CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		pImpl->CreatePipelineState();
		///�R�}���h���X�g�쐬
		pImpl->CreateCommandList();
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());

	}
	void PCDynamicDraw::OnDraw() {
		auto Mesh = GetMeshResource();
		if (!Mesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!Mesh)",
				L"PCDynamicDraw::OnDraw()"
			);
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
		pImpl->DrawObject(Mesh, GetGameObject()->IsAlphaActive());

	}

	//--------------------------------------------------------------------------------------
	//	struct PTDynamicDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PTDynamicDraw::Impl {
		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		bool m_Trace;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		///�e�������֐�
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		bool m_Wrap;
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView(const shared_ptr<TextureResource>& TextureRes);
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		///�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix);
		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace,
			const shared_ptr<TextureResource>& TextureRes, bool WrapSampler);
		Impl() :
			m_Trace(false),
			m_Wrap(false)
		{}
	};

	///���[�g�V�O�l�`���쐬
	void PTDynamicDraw::Impl::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void PTDynamicDraw::Impl::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
		//�T���v���[�f�X�N�v���^�q�[�v
		m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�T���v���[�쐬
	void PTDynamicDraw::Impl::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///�V�F�[�_�[���\�[�X�r���[�쐬
	void PTDynamicDraw::Impl::CreateShaderResourceView(const shared_ptr<TextureResource>& TextureRes) {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = TextureRes->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = TextureRes->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			TextureRes->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PTDynamicDraw::Impl::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(StaticConstantBuffer) + 255) & ~255;

		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"PCTDynamicDraw::Impl::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"PCTDynamicDraw::Impl::CreateConstantBuffer()"
		);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void PTDynamicDraw::Impl::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault3D<VertexPositionTexture, VSPTStatic, PSPTStatic>(m_RootSignature, PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void PTDynamicDraw::Impl::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///�R���X�^���g�o�b�t�@�X�V
	void PTDynamicDraw::Impl::UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix) {
		//�s��̒�`
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
		//�]�u����
		World.Transpose();
		//�J�����𓾂�
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.Transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();
		//�R���X�^���g�o�b�t�@�̏���
		StaticConstantBuffer cb;
		cb.World = World;
		cb.View = ViewMat;
		cb.Projection = ProjMat;
		//�G�~�b�V�u
		cb.Emissive = Emissive;
		//�f�t�B�[�Y�͂��ׂĒʂ�
		cb.Diffuse = Diffuse;
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&cb),
			sizeof(StaticConstantBuffer));

	}
	///�`�揈��
	void PTDynamicDraw::Impl::DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace,
		const shared_ptr<TextureResource>& TextureRes, bool WrapSampler) {
		if (m_Wrap != WrapSampler) {
			auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			if (WrapSampler) {
				//���b�s���O�T���v���[
				DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
			}
			else {
				DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
			}
			m_Wrap = WrapSampler;
		}
		if (m_Trace != Trace) {
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
			m_PipelineState = PipelineState::CreateDefault3D<VertexPositionTexture, VSPTStatic, PSPTStatic>(m_RootSignature, PineLineDesc);
			//�����̏ꍇ�̓u�����h�X�e�[�g�����ւ�
			if (Trace) {
				D3D12_BLEND_DESC blend_desc;
				D3D12_RENDER_TARGET_BLEND_DESC Target;
				ZeroMemory(&blend_desc, sizeof(blend_desc));
				blend_desc.AlphaToCoverageEnable = false;
				blend_desc.IndependentBlendEnable = false;
				ZeroMemory(&Target, sizeof(Target));
				Target.BlendEnable = true;
				Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
				Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				Target.BlendOp = D3D12_BLEND_OP_ADD;
				Target.SrcBlendAlpha = D3D12_BLEND_ONE;
				Target.DestBlendAlpha = D3D12_BLEND_ZERO;
				Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
				Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
					blend_desc.RenderTarget[i] = Target;
				}
				PineLineDesc.BlendState = blend_desc;
				m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
				m_Trace = Trace;
			}
		}
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

		MeshRes->UpdateResources<VertexPositionTexture>(m_CommandList);
		TextureRes->UpdateResources(m_CommandList);

		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&MeshRes->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &MeshRes->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(MeshRes->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}


	//--------------------------------------------------------------------------------------
	///	PTDynamicDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PTDynamicDraw::PTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DynamicBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PTDynamicDraw::~PTDynamicDraw() {}

	void PTDynamicDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		TextureDrawInterface::SetTextureResource(TextureRes);
		pImpl->CreateShaderResourceView(TextureRes);
	}
	void PTDynamicDraw::SetTextureResource(const wstring& TextureKey) {
		TextureDrawInterface::SetTextureResource(TextureKey);
		pImpl->CreateShaderResourceView(GetTextureResource());
	}


	void PTDynamicDraw::CreateMesh(vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//���b�V���̍쐬�i�ύX�ł���j
			auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(MeshRes);
		}
		catch (...) {
			throw;
		}
	}

	void PTDynamicDraw::UpdateVertices(const vector<VertexPositionTexture>& Vertices) {
		auto Mesh = GetMeshResource();
		if (!Mesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!Mesh)",
				L"PTDynamicDraw::UpdateVertices()"
			);
		}
		//���b�V���̒��_�̕ύX
		Mesh->UpdateVirtex(Vertices);
	}

	void PTDynamicDraw::OnCreate() {
		///���[�g�V�O�l�`���쐬
		pImpl->CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		pImpl->CreateDescriptorHeap();
		///�T���v���[�쐬
		pImpl->CreateSampler();
		///�R���X�^���g�o�b�t�@�쐬
		pImpl->CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		pImpl->CreatePipelineState();
		///�R�}���h���X�g�쐬
		pImpl->CreateCommandList();
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}
	void PTDynamicDraw::OnDraw() {
		auto Mesh = GetMeshResource();
		if (!Mesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!Mesh)",
				L"PTDynamicDraw::OnDraw()"
			);
		}
		//�e�N�X�`�����Ȃ���Ε`�悵�Ȃ�
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
		pImpl->DrawObject(Mesh, GetGameObject()->IsAlphaActive(),
			shTex, GetWrapSampler());

	}





	//--------------------------------------------------------------------------------------
	//	struct PCTDynamicDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCTDynamicDraw::Impl {
		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		bool m_Trace;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		///�e�������֐�
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		bool m_Wrap;
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView(const shared_ptr<TextureResource>& TextureRes);
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();
		///�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
			const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix);
		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace,
			const shared_ptr<TextureResource>& TextureRes, bool WrapSampler);
		Impl() :
			m_Trace(false),
			m_Wrap(false)
		{}
	};

	///���[�g�V�O�l�`���쐬
	void PCTDynamicDraw::Impl::CreateRootSignature() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
	}
	///�f�X�N�v���^�q�[�v�쐬
	void PCTDynamicDraw::Impl::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//CbvSrv�f�X�N�v���^�q�[�v
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
		//�T���v���[�f�X�N�v���^�q�[�v
		m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�T���v���[�쐬
	void PCTDynamicDraw::Impl::CreateSampler() {
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
	}
	///�V�F�[�_�[���\�[�X�r���[�쐬
	void PCTDynamicDraw::Impl::CreateShaderResourceView(const shared_ptr<TextureResource>& TextureRes) {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = TextureRes->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = TextureRes->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			TextureRes->GetTexture().Get(),
			&srvDesc,
			Handle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void PCTDynamicDraw::Impl::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(StaticConstantBuffer) + 255) & ~255;

		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"PCTDynamicDraw::Impl::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"PCTDynamicDraw::Impl::CreateConstantBuffer()"
		);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void PCTDynamicDraw::Impl::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault3D<VertexPositionColorTexture, VSPCTStatic, PSPCTStatic>(m_RootSignature, PineLineDesc);
	}
	///�R�}���h���X�g�쐬
	void PCTDynamicDraw::Impl::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///�R���X�^���g�o�b�t�@�X�V
	void PCTDynamicDraw::Impl::UpdateConstantBuffer(const shared_ptr<GameObject>& GameObjectPtr,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix) {
		//�s��̒�`
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
		//�]�u����
		World.Transpose();
		//�J�����𓾂�
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.Transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.Transpose();
		//�R���X�^���g�o�b�t�@�̏���
		StaticConstantBuffer cb;
		cb.World = World;
		cb.View = ViewMat;
		cb.Projection = ProjMat;
		//�G�~�b�V�u
		cb.Emissive = Emissive;
		//�f�t�B�[�Y�͂��ׂĒʂ�
		cb.Diffuse = Diffuse;
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&cb),
			sizeof(StaticConstantBuffer));

	}
	///�`�揈��
	void PCTDynamicDraw::Impl::DrawObject(const shared_ptr<MeshResource>& MeshRes, bool Trace,
		const shared_ptr<TextureResource>& TextureRes, bool WrapSampler) {
		if (m_Wrap != WrapSampler) {
			auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			if (WrapSampler) {
				//���b�s���O�T���v���[
				DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
			}
			else {
				DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
			}
			m_Wrap = WrapSampler;
		}
		if (m_Trace != Trace) {
			D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
			m_PipelineState = PipelineState::CreateDefault3D<VertexPositionColorTexture, VSPCTStatic, PSPCTStatic>(m_RootSignature, PineLineDesc);
			//�����̏ꍇ�̓u�����h�X�e�[�g�����ւ�
			if (Trace) {
				D3D12_BLEND_DESC blend_desc;
				D3D12_RENDER_TARGET_BLEND_DESC Target;
				ZeroMemory(&blend_desc, sizeof(blend_desc));
				blend_desc.AlphaToCoverageEnable = false;
				blend_desc.IndependentBlendEnable = false;
				ZeroMemory(&Target, sizeof(Target));
				Target.BlendEnable = true;
				Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
				Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				Target.BlendOp = D3D12_BLEND_OP_ADD;
				Target.SrcBlendAlpha = D3D12_BLEND_ONE;
				Target.DestBlendAlpha = D3D12_BLEND_ZERO;
				Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
				Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
					blend_desc.RenderTarget[i] = Target;
				}
				PineLineDesc.BlendState = blend_desc;
				m_PipelineState = PipelineState::CreateDirect(PineLineDesc);
				m_Trace = Trace;
			}
		}
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

		MeshRes->UpdateResources<VertexPositionColorTexture>(m_CommandList);
		TextureRes->UpdateResources(m_CommandList);

		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&MeshRes->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &MeshRes->GetVertexBufferView());
		m_CommandList->DrawIndexedInstanced(MeshRes->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}


	//--------------------------------------------------------------------------------------
	///	PCTDynamic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCTDynamicDraw::PCTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DynamicBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��3D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCTDynamicDraw::~PCTDynamicDraw() {}

	void PCTDynamicDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		TextureDrawInterface::SetTextureResource(TextureRes);
		pImpl->CreateShaderResourceView(TextureRes);
	}
	void PCTDynamicDraw::SetTextureResource(const wstring& TextureKey) {
		TextureDrawInterface::SetTextureResource(TextureKey);
		pImpl->CreateShaderResourceView(GetTextureResource());
	}


	void PCTDynamicDraw::CreateMesh(vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) {
		try {
			//���b�V���̍쐬�i�ύX�ł���j
			auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
			SetMeshResource(MeshRes);
		}
		catch (...) {
			throw;
		}
	}

	void PCTDynamicDraw::UpdateVertices(const vector<VertexPositionColorTexture>& Vertices) {
		auto Mesh = GetMeshResource();
		if (!Mesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!Mesh)",
				L"PCTDynamicDraw::UpdateVertices()"
			);
		}
		//���b�V���̒��_�̕ύX
		Mesh->UpdateVirtex(Vertices);
	}

	void PCTDynamicDraw::OnCreate() {
		///���[�g�V�O�l�`���쐬
		pImpl->CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		pImpl->CreateDescriptorHeap();
		///�T���v���[�쐬
		pImpl->CreateSampler();
		///�R���X�^���g�o�b�t�@�쐬
		pImpl->CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		pImpl->CreatePipelineState();
		///�R�}���h���X�g�쐬
		pImpl->CreateCommandList();
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
	}
	void PCTDynamicDraw::OnDraw() {
		auto Mesh = GetMeshResource();
		if (!Mesh) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!Mesh)",
				L"PCTDynamicDraw::OnDraw()"
			);
		}
		//�e�N�X�`�����Ȃ���Ε`�悵�Ȃ�
		auto shTex = GetTextureResource();
		if (!shTex) {
			return;
		}
		//�R���X�^���g�o�b�t�@�̍X�V
		pImpl->UpdateConstantBuffer(GetGameObject(),
			GetEmissive(), GetDiffuse(), GetMeshToTransformMatrix());
		pImpl->DrawObject(Mesh, GetGameObject()->IsAlphaActive(),
			shTex, GetWrapSampler());

	}

	//--------------------------------------------------------------------------------------
	//	struct PNTDynamicDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PNTDynamicDraw::Impl {
		bool m_OwnShadowActive;
		Impl() :
			m_OwnShadowActive(false)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	PNTDynamic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTDynamicDraw::PNTDynamicDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DynamicBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
	}

	PNTDynamicDraw::~PNTDynamicDraw() {}

	void PNTDynamicDraw::CreateMesh(vector<VertexPositionNormalTexture>& Vertices, vector<uint16_t>& indices) {
	}

	void PNTDynamicDraw::UpdateVertices(const vector<VertexPositionNormalTexture>& Vertices) {
	}


	bool PNTDynamicDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTDynamicDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTDynamicDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}



	void PNTDynamicDraw::OnCreate() {
	}

	void PNTDynamicDraw::DrawNotShadow() {
	}

	void PNTDynamicDraw::DrawWithShadow() {
	}

	void PNTDynamicDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}



	//--------------------------------------------------------------------------------------
	//	struct StaticBaseDraw::Impl;
	//	�p�r: StaticBaseDraw�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct StaticBaseDraw::Impl {
		//���b�V����weak_ptr
		weak_ptr<MeshResource> m_MeshResource;
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	static�`��R���|�[�l���g�̐e(���_��ύX�ł��Ȃ�3D�`��)
	//--------------------------------------------------------------------------------------
	StaticBaseDraw::StaticBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		Base3DDraw(GameObjectPtr),
		pImpl(new Impl())
	{}
	StaticBaseDraw::~StaticBaseDraw() {}
	shared_ptr<MeshResource> StaticBaseDraw::GetMeshResource() const {
		auto shptr = pImpl->m_MeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}
	void StaticBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void StaticBaseDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}

	//--------------------------------------------------------------------------------------
	//	struct PCStaticDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCStaticDraw::Impl {
		Impl()
		{}
	};



	//--------------------------------------------------------------------------------------
	///	PCStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCStaticDraw::PCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
	}

	PCStaticDraw::~PCStaticDraw() {}

	void PCStaticDraw::OnDraw() {
	}


	//--------------------------------------------------------------------------------------
	//	struct PTStaticDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PTStaticDraw::Impl {
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PTStaticDraw::PTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
	}

	PTStaticDraw::~PTStaticDraw() {}


	void PTStaticDraw::OnDraw() {
	}


	//--------------------------------------------------------------------------------------
	//	struct PCTStaticDraw::Impl;
	//--------------------------------------------------------------------------------------
	struct PCTStaticDraw::Impl {
		Impl()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PCTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCTStaticDraw::PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		StaticBaseDraw(GameObjectPtr),
		pImpl(new Impl())
	{
	}

	PCTStaticDraw::~PCTStaticDraw() {}


	void PCTStaticDraw::OnDraw() {
	}


	//--------------------------------------------------------------------------------------
	//	struct PNTStaticDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PNTStaticDraw::Impl {

		//���b�V�����\�[�X
		weak_ptr<MeshResource> m_MeshResource;
		//�e�N�X�`��
		weak_ptr<TextureResource> m_TextureResource;
		bool m_OwnShadowActive;
		bool m_Trace;

		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///Sampler�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_SamplerDescriptorHandleIncrementSize{ 0 };

		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///�R���X�^���g�o�b�t�@
		struct StaticConstantBuffer
		{
			bsm::Mat4x4 World;
			bsm::Mat4x4 View;
			bsm::Mat4x4 Projection;
			bsm::Vec4 LightDir;
			bsm::Col4 Emissive;
			bsm::Col4 Diffuse;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		///�R���X�^���g�o�b�t�@�̃I�u�W�F�N�g���ϐ�
		StaticConstantBuffer m_StaticConstantBuffer;
		///�e�t���R���X�^���g�o�b�t�@
		struct StaticConstantBufferWithShadow
		{
			bsm::Mat4x4 World;
			bsm::Mat4x4 View;
			bsm::Mat4x4 Projection;
			bsm::Vec4 LightDir;
			bsm::Col4 Emissive;
			bsm::Col4 Diffuse;
			bsm::Vec4 LightPos;
			bsm::Vec4 EyePos;
			XMUINT4 ActiveFlg;			//�e�N�X�`��=x���A�N�e�B�u���ǂ���
			bsm::Mat4x4 LightView;
			bsm::Mat4x4 LightProjection;
			StaticConstantBufferWithShadow() {
				memset(this, 0, sizeof(StaticConstantBufferWithShadow));
			};
		};
		///�R���X�^���g�o�b�t�@�̃I�u�W�F�N�g���ϐ�
		StaticConstantBufferWithShadow m_StaticConstantBufferWithShadow;


		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeapWithShadow;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		void* m_pConstantBufferWithShadow{ nullptr };
		///�p�C�v���C���X�e�[�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PineLineDesc;

		ComPtr<ID3D12PipelineState> m_CullBackPipelineState;
		ComPtr<ID3D12PipelineState> m_CullFrontPipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		///�������֐�
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView(bool IsShadow);
		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer(bool IsShadow);
		///�`�揈��
		void DrawObject();

		void DrawObjectWithShadow();


		///�v���p�e�B�ύX
		void RefreshTrace();


		Impl() :
			m_OwnShadowActive(false), m_Trace(false)
		{
			//�e�s���Identity�ɏ�����
			ZeroMemory(&m_StaticConstantBuffer, sizeof(m_StaticConstantBuffer));
			m_StaticConstantBuffer.World = bsm::Mat4x4EX::Identity();
			m_StaticConstantBuffer.View = bsm::Mat4x4EX::Identity();
			m_StaticConstantBuffer.Projection = bsm::Mat4x4EX::Identity();
			m_StaticConstantBuffer.Emissive = bsm::Col4(0.0f, 0.0f, 0.0f, 0.0f);
			m_StaticConstantBuffer.Diffuse = bsm::Col4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f);
		}
		~Impl() {}
	};


	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void PNTStaticDraw::Impl::CreateShaderResourceView(bool IsShadow) {
		auto ShPtr = m_TextureResource.lock();
		if (!ShPtr) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = ShPtr->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = ShPtr->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			ShPtr->GetTexture().Get(),
			&srvDesc,
			Handle);

		if (IsShadow) {
			auto ShdowRender = Dev->GetShadowMapRenderTarget();

			CD3DX12_CPU_DESCRIPTOR_HANDLE ShadowHandle(
				m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				m_CbvSrvDescriptorHandleIncrementSize
			);

			D3D12_SHADER_RESOURCE_VIEW_DESC shadowSrvDesc = {};
			shadowSrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
			shadowSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			shadowSrvDesc.Texture2D.MipLevels = 1;
			shadowSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			Dev->GetDevice()->CreateShaderResourceView(ShdowRender->GetDepthStencil().Get(), &shadowSrvDesc, ShadowHandle);
		}
	}

	void PNTStaticDraw::Impl::RefreshTrace() {
		//�u�����h�X�e�[�g�ƃ��X�^���C�U�����ւ�
		if (m_Trace) {
			D3D12_BLEND_DESC blend_desc;
			D3D12_RENDER_TARGET_BLEND_DESC Target;
			ZeroMemory(&blend_desc, sizeof(blend_desc));
			blend_desc.AlphaToCoverageEnable = false;
			blend_desc.IndependentBlendEnable = false;
			ZeroMemory(&Target, sizeof(Target));
			Target.BlendEnable = true;
			Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			Target.BlendOp = D3D12_BLEND_OP_ADD;
			Target.SrcBlendAlpha = D3D12_BLEND_ONE;
			Target.DestBlendAlpha = D3D12_BLEND_ZERO;
			Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
				blend_desc.RenderTarget[i] = Target;
			}
			m_PineLineDesc.BlendState = blend_desc;
		}
		else {
			m_PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		}
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		m_CullFrontPipelineState = PipelineState::CreateDirect(m_PineLineDesc);

		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(m_PineLineDesc);

	}

	void PNTStaticDraw::Impl::UpdateConstantBuffer(bool IsShadow) {
		if (IsShadow) {
			memcpy(m_pConstantBufferWithShadow, reinterpret_cast<void**>(&m_StaticConstantBufferWithShadow),
				sizeof(m_StaticConstantBufferWithShadow));
		}
		else {
			memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_StaticConstantBuffer),
				sizeof(m_StaticConstantBuffer));
		}
	}


	///�`�揈��
	void PNTStaticDraw::Impl::DrawObject() {
		auto ShMesh = m_MeshResource.lock();
		auto ShTex = m_TextureResource.lock();
		if (!ShMesh || !ShTex) {
			return;
		}

		//�R�}���h���X�g�̃��Z�b�g
		if (m_Trace) {
			CommandList::Reset(m_CullFrontPipelineState, m_CommandList);
		}
		else {
			CommandList::Reset(m_CullBackPipelineState, m_CommandList);
		}

		ShMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		ShTex->UpdateResources(m_CommandList);

		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());



		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());


		m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (m_Trace) {
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		}

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}

	void PNTStaticDraw::Impl::DrawObjectWithShadow() {
		auto ShMesh = m_MeshResource.lock();
		auto ShTex = m_TextureResource.lock();
		if (!ShMesh || !ShTex) {
			return;
		}

		//�R�}���h���X�g�̃��Z�b�g
		if (m_Trace) {
			CommandList::Reset(m_CullFrontPipelineState, m_CommandList);
		}
		else {
			CommandList::Reset(m_CullBackPipelineState, m_CommandList);
		}

		ShMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		ShTex->UpdateResources(m_CommandList);

		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());



		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		


		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());



		m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (m_Trace) {
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		}
		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());

	}






	//--------------------------------------------------------------------------------------
	//	class PNTStaticDraw : public DrawComponent;
	//	�p�r: PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl()) {
		//�p�C�v���C���X�e�[�g���f�t�H���g�̂RD
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::CreateNotShadow() {
		if (ObjectInterface::IsCreated() && !pImpl->m_OwnShadowActive) {
			return;
		}
		//���[�g�V�O�l�`��
		pImpl->m_RootSignature = RootSignature::CreateSrvSmpCbv();
		//�f�X�N�v���^�q�[�v
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_CbvSrvDescriptorHandleIncrementSize =
				Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//CbvSrv�f�X�N�v���^�q�[�v
			pImpl->m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//�T���v���[�f�X�N�v���^�q�[�v
			pImpl->m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			pImpl->m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
				pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_GPUDescriptorHandleVec.push_back(SrvHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
				pImpl->m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_GPUDescriptorHandleVec.push_back(SamplerHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_CbvSrvDescriptorHandleIncrementSize
			);
			pImpl->m_GPUDescriptorHandleVec.push_back(CbvHandle);

		}
		//�T���v���[
		{
			auto SamplerDescriptorHandle = pImpl->m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
		}
		//�R���X�^���g�o�b�t�@
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(pImpl->m_StaticConstantBuffer) + 255) & ~255;
			//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
			ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_ConstantBufferUploadHeap)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"PNTStaticDraw::CreateNotShadow()"
			);
			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
			//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_CbvSrvDescriptorHandleIncrementSize
			);

			Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&pImpl->m_pConstantBuffer)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"PNTStaticDraw::CreateNotShadow()"
			);

		}
		//�V�F�[�_���\�[�X�r���[�̓e�N�X�`���Z�b�g���ɍ쐬
		//�p�C�v���C���X�e�[�g�̍쐬
		{
			PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(pImpl->m_RootSignature, pImpl->m_PineLineDesc);
			pImpl->m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_PineLineDesc);
			pImpl->m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_PineLineDesc);
		}
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		{
			pImpl->m_CommandList = CommandList::CreateDefault(pImpl->m_CullBackPipelineState);
			pImpl->UpdateConstantBuffer(false);
			CommandList::Close(pImpl->m_CommandList);
		}
		//�R���X�^���g�o�b�t�@�X�V
		pImpl->UpdateConstantBuffer(false);
		pImpl->m_OwnShadowActive = false;
	}

	void PNTStaticDraw::CreateWithShadow() {
		if (ObjectInterface::IsCreated() && pImpl->m_OwnShadowActive) {
			return;
		}
		//���[�g�V�O�l�`���V���h�E�t��
		pImpl->m_RootSignature = RootSignature::CreateSrv2Smp2Cbv();
		//�f�X�N�v���^�q�[�v
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_CbvSrvDescriptorHandleIncrementSize =
				Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			pImpl->m_SamplerDescriptorHandleIncrementSize =
				Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

			//CbvSrv�f�X�N�v���^�q�[�v
			pImpl->m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 2);
			//�T���v���[�f�X�N�v���^�q�[�v
			pImpl->m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(2);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			pImpl->m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle1(
				pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_GPUDescriptorHandleVec.push_back(SrvHandle1);
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle2(
				pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_CbvSrvDescriptorHandleIncrementSize
			);
			pImpl->m_GPUDescriptorHandleVec.push_back(SrvHandle2);

			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle1(
				pImpl->m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_GPUDescriptorHandleVec.push_back(SamplerHandle1);

			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle2(
				pImpl->m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_SamplerDescriptorHandleIncrementSize
			);
			pImpl->m_GPUDescriptorHandleVec.push_back(SamplerHandle2);


			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				2,
				pImpl->m_CbvSrvDescriptorHandleIncrementSize
			);
			pImpl->m_GPUDescriptorHandleVec.push_back(CbvHandle);
		}
		//�T���v���[
		{
			auto SamplerDescriptorHandleWithShadow1 = pImpl->m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandleWithShadow1);

			CD3DX12_CPU_DESCRIPTOR_HANDLE SamplerDescriptorHandleWithShadow2{
				pImpl->m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_SamplerDescriptorHandleIncrementSize
			};
			DynamicSampler::CreateSampler(SamplerState::ComparisonLinear, SamplerDescriptorHandleWithShadow2);
		}
		//�R���X�^���g�o�b�t�@
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(pImpl->m_StaticConstantBufferWithShadow) + 255) & ~255;
			//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
			ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_ConstantBufferUploadHeapWithShadow)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"PNTStaticDraw::CreateWithShadow()"
			);
			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_ConstantBufferUploadHeapWithShadow->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
			//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				2,
				pImpl->m_CbvSrvDescriptorHandleIncrementSize
			);

			Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_ConstantBufferUploadHeapWithShadow->Map(0, &readRange, reinterpret_cast<void**>(&pImpl->m_pConstantBufferWithShadow)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeapWithShadow->Map()",
				L"PNTStaticDraw::CreateWithShadow()"
			);

		}
		//�V�F�[�_���\�[�X�r���[�̓e�N�X�`���Z�b�g���ɍ쐬
		//�p�C�v���C���X�e�[�g�̍쐬
		{
			//�V���h�E�t���p�C�v���C���X�e�[�g�̍쐬
			PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStaticShadow, PSPNTStaticShadow>(pImpl->m_RootSignature,
				pImpl->m_PineLineDesc);
			pImpl->m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_PineLineDesc);
			pImpl->m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_PineLineDesc);
		}
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		{
			pImpl->m_CommandList = CommandList::CreateDefault(pImpl->m_CullBackPipelineState);
			pImpl->UpdateConstantBuffer(true);
			CommandList::Close(pImpl->m_CommandList);
		}
		//�R���X�^���g�o�b�t�@�X�V
		pImpl->UpdateConstantBuffer(true);
		pImpl->m_OwnShadowActive = true;

	}



	void PNTStaticDraw::OnCreate() {
		//�e�����ɏ�����
		CreateNotShadow();
	}

	shared_ptr<MeshResource> PNTStaticDraw::GetMeshResource() const {
		//���b�V�����Ȃ���΃��^�[��
		auto shptr = pImpl->m_MeshResource.lock();
		if (!shptr) {
			throw BaseException(
				L"���b�V�����ݒ肳��Ă܂���",
				L"if (!shptr)",
				L"PNTStaticDraw::GetMeshResource()"
			);
		}
		return shptr;
	}

	void PNTStaticDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}
	void PNTStaticDraw::SetMeshResource(const wstring& MeshKey) {
		pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(MeshKey);
	}
	void PNTStaticDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
		pImpl->CreateShaderResourceView(IsOwnShadowActive());
	}
	void PNTStaticDraw::SetTextureResource(const wstring& TextureKey) {
		auto Res = App::GetApp()->GetResource<TextureResource>(TextureKey);
		pImpl->m_TextureResource = Res;
		pImpl->CreateShaderResourceView(IsOwnShadowActive());
	}
	shared_ptr<TextureResource> PNTStaticDraw::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = pImpl->m_TextureResource.lock();
		if (!shptr){
			return nullptr;
		}
		return pImpl->m_TextureResource.lock();
	}

	bsm::Col4 PNTStaticDraw::GetEmissive() const {
		return pImpl->m_StaticConstantBuffer.Emissive;
	}
	void PNTStaticDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_StaticConstantBuffer.Emissive = col;
	}

	bool PNTStaticDraw::GetAlphaActive() const {
		return pImpl->m_Trace;

	}
	void PNTStaticDraw::SetAlphaActive(bool b) {
		if (b != pImpl->m_Trace) {
			pImpl->m_Trace = b;
			pImpl->RefreshTrace();
		}
	}

	bool PNTStaticDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	bool PNTStaticDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}
	void PNTStaticDraw::SetOwnShadowActive(bool b) {
		if (b) {
			CreateWithShadow();
		}
		else {
			CreateNotShadow();
		}
	}


	void PNTStaticDraw::DrawNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		SetAlphaActive(GetGameObject()->GetAlphaActive());
		auto PtrMeshResource = GetMeshResource();
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//���[���h�s��̌���
		pImpl->m_StaticConstantBuffer.World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//�]�u����
		pImpl->m_StaticConstantBuffer.World.Transpose();
		//�r���[�s��̌���
		auto StageView = PtrStage->GetView();
		pImpl->m_StaticConstantBuffer.View = StageView->GetTargetCamera()->GetViewMatrix();
		//�]�u����
		pImpl->m_StaticConstantBuffer.View.Transpose();
		//�ˉe�s��̌���
		pImpl->m_StaticConstantBuffer.Projection = StageView->GetTargetCamera()->GetProjMatrix();
		//�]�u����
		pImpl->m_StaticConstantBuffer.Projection.Transpose();



		//���C�e�B���O
		auto StageLight = PtrStage->GetLight();
		pImpl->m_StaticConstantBuffer.LightDir = StageLight->GetTargetLight().m_Directional;
		//�G�~�b�V�u
		pImpl->m_StaticConstantBuffer.Emissive = GetEmissive();
		//�f�B�t���[�Y
		pImpl->m_StaticConstantBuffer.Diffuse = StageLight->GetTargetLight().m_DiffuseColor;
		//�X�V
		pImpl->UpdateConstantBuffer(false);
		pImpl->DrawObject();
	}

	void PNTStaticDraw::DrawWithShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		SetAlphaActive(GetGameObject()->GetAlphaActive());
		auto PtrMeshResource = GetMeshResource();
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//���[���h�s��̌���
		pImpl->m_StaticConstantBufferWithShadow.World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//�]�u����
		pImpl->m_StaticConstantBufferWithShadow.World.Transpose();
		//�r���[�s��̌���
		auto StageView = PtrStage->GetView();
		pImpl->m_StaticConstantBufferWithShadow.View = StageView->GetTargetCamera()->GetViewMatrix();
		//�]�u����
		pImpl->m_StaticConstantBufferWithShadow.View.Transpose();
		//�ˉe�s��̌���
		pImpl->m_StaticConstantBufferWithShadow.Projection = StageView->GetTargetCamera()->GetProjMatrix();
		//�]�u����
		pImpl->m_StaticConstantBufferWithShadow.Projection.Transpose();
		//���C�e�B���O
		auto StageLight = PtrStage->GetLight();
		pImpl->m_StaticConstantBufferWithShadow.LightDir = StageLight->GetTargetLight().m_Directional;
		//�G�~�b�V�u
		pImpl->m_StaticConstantBufferWithShadow.Emissive = GetEmissive();
		//�f�B�t���[�Y
		pImpl->m_StaticConstantBufferWithShadow.Diffuse = StageLight->GetTargetLight().m_DiffuseColor;



		bsm::Vec3 CalcLightDir = -1.0 * StageLight->GetTargetLight().m_Directional;
		bsm::Vec3 LightAt = StageView->GetTargetCamera()->GetAt();
		bsm::Vec3 LightEye = CalcLightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		pImpl->m_StaticConstantBufferWithShadow.LightPos = LightEye;
		pImpl->m_StaticConstantBufferWithShadow.LightPos.w = 1.0f;

		pImpl->m_StaticConstantBufferWithShadow.EyePos = StageView->GetTargetCamera()->GetEye();
		pImpl->m_StaticConstantBufferWithShadow.EyePos.w = 1.0f;
		pImpl->m_StaticConstantBufferWithShadow.ActiveFlg.x = 1;

		bsm::Mat4x4 LightView, LightProj;
		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView.LookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		pImpl->m_StaticConstantBufferWithShadow.LightView = bsm::Mat4x4EX::Transpose(LightView);
		pImpl->m_StaticConstantBufferWithShadow.LightProjection = bsm::Mat4x4EX::Transpose(LightProj);

		//�X�V
		pImpl->UpdateConstantBuffer(true);
		pImpl->DrawObjectWithShadow();

	}


	void PNTStaticDraw::OnDraw() {
		if (GetOwnShadowActive()) {
			DrawWithShadow();
		}
		else {
			DrawNotShadow();
		}
	}



}
//end basecross
