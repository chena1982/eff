/******************************************************************************
	created:	2008-12-16   23:24
	file path:	d:\EFF\EFFEngine\Include\EFF3D\EFF3DResource.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFF3DResource_2008_12_16__
#define __EFF3DResource_2008_12_16__

EFF3D_BEGIN


class EFF3DResourceManager;

class EFF3DDevice;



//��Դ�ֳ�������Դ�͸��ֱ�������ʱ��Դ(��������Ĳ�̫��)���������Կ��е���ͼ(����ָӲ�̻��ڴ������ͼ)
//VB��IB��RenderTarget��
//��Դ��ͬ�����ط�����:
//   1.���ڴ����Ӳ�̶�ȡ������Դ
//   2.��������ʱ��Դ
//��Щ��������Ⱦ�߳���ִ��

//��Դ���첽���ؾͱȽϸ�����
//   1.����һ��Request���͵�io�߳�
//   2.io�߳��ں�̨��ȡ������Դ
//   3.���ݴ����̶߳Ըն�ȡ����������Ҫ�Ĵ��������ѹ���ȵ�
//   4.�ٷ��͵���Ⱦ�̣߳���������ʱ��Դ������Դ��������ΪD3DҪ֧�ֶ��̻߳���ɺܴ�������½�(��������Ⱦ�̴߳�������������ʱ��Դ)
//   5.����б�Ҫ����io�߳�������ݿ���������ʱ��Դ��(���ʹ���ļ�ӳ��Ļ�����2��ʵ�ʲ�û��ִ��������io��
//      �����3��Ҳû��ִ���κβ�������ô������io������һ����ִ�У������б�Ҫ�����ݿ����ŵ�5ִ�У�������ֱ���ڵ�4��ִ��)
//   6.���ݿ������֮���������ʱ��Դ
//����ioֻ��һ���̣߳����ݴ����߳̿����ж��

/*class EFF3D_API EFF3DIResource : public EFFComponent
{
	RTTI_DECLARE_PURE(EFF3DIResource, EFFComponent)



	friend class EFF3DAsyncLoader;
public:

	virtual effVOID								SetOrigin(const effString & strOrigin) = 0;
	virtual const effString &					GetOrigin() const = 0;
	virtual effVOID								SetName(const effString & strName) = 0;
	virtual const effString &					GetName() const = 0;
	virtual effVOID								SetResourceManager(EFF3DResourceManager * pManager) = 0;
	virtual EFF3DResourceManager *				GetResourceManager() const = 0;
	virtual effVOID								SetUnloaded(effBOOL bUnloaded) = 0;
	virtual effBOOL								IsUnloaded() const = 0;
	virtual effBOOL								IsLoaded() const = 0;

	virtual effUINT								CalculateSize() = 0;
	virtual effUINT								GetMemorySize() const = 0;
	virtual effUINT								GetVideoMemorySize() const  = 0;
	virtual effUINT								GetAGPMemorySize() const = 0;

//	virtual effBOOL								CreateFromFile(const effString & strFilePath,EFF3DDevice * pDevice) = 0;
//	virtual effBOOL								AsyncCreateFromFile(const effString & strFilePath) = 0;
	virtual effBOOL								Reload() = 0;
	virtual effVOID								Unload() = 0;

protected:
	virtual effBOOL								LoadDataFromFile(const effString & strFilePath) = 0;
	virtual effBOOL								Process() = 0;
	virtual effBOOL								CreateRuntimeResource(EFF3DDevice * pDevice) = 0;

	virtual effHRESULT							Lock() = 0;
	virtual effBOOL								CopyDataToRuntimeResource() = 0;
	virtual effHRESULT							Unlock() = 0;

	virtual effVOID								LoadResourceError() = 0;
	virtual effVOID								LoadResourceEnd() = 0;
};*/

class EFF3D_API EFF3DResource : public EFFObject
{

	RTTI_DECLARE(EFF3DResource, EFFObject)

protected:
	EFF3DResource() : isUnloaded(effFALSE), isLoaded(effFALSE), memorySize(0), videoMemorySize(0),
					AGPMeorySize(0), manager(NULL), data(NULL), runtimeResourceLockPtr(NULL),
					dataSize(0)
	{ 
	}
public:
	virtual ~EFF3DResource() {}


public:



	effVOID SetOrigin(const effString & originPath) { this->originPath = originPath; }
	const effString & GetOriginPath() const { return originPath; }

	effVOID SetName(const effString & name) { this->name = name; }
	const effString & GetName() const { return name; }

	effVOID SetResourceManager(EFF3DResourceManager * manager) { this->manager = manager; }
	EFF3DResourceManager * GetResourceManager() const { return manager; }

	effVOID SetUnloaded(effBOOL isUnloaded) { this->isUnloaded = isUnloaded; }
	effBOOL IsUnloaded() const { return isUnloaded; }

	effBOOL IsLoaded() const { return isLoaded; }

	effUINT GetMemorySize() const { return memorySize; }
	effUINT GetVideoMemorySize() const { return videoMemorySize; }
	effUINT GetAGPMemorySize() const { return AGPMeorySize; }

public:

	virtual effBOOL					LoadDataFromFile(const effString & filePath) { return effTRUE; }
	virtual effBOOL					Process() { return effTRUE; }
	virtual effBOOL					CreateRuntimeResource(EFF3DDevice * pDevice) { return effTRUE; }

	virtual effBOOL					Lock() { return effTRUE; }
	virtual effBOOL					CopyDataToRuntimeResource() { return effTRUE; }
	virtual effBOOL					Unlock() { return effTRUE; }

	virtual effVOID					LoadResourceError() {}
	virtual effVOID					LoadResourceEnd() { isLoaded = effTRUE; }

	virtual effVOID					CalculateSize() {}
public:
	EFFEvent						OnBeforeCreate;
	EFFEvent						OnAfterCreate;

	EFFEvent						OnUnload;

protected:
	effString						name;
	effString						groupName;
	effBOOL							isUnloaded;
	effBOOL							isLoaded;
	effUINT							memorySize;
	effUINT							videoMemorySize;
	effUINT							AGPMeorySize;
	effString						originPath;
	EFF3DResourceManager *			manager;
	effBYTE *						data;
	effUINT							dataSize;
	effVOID *						runtimeResourceLockPtr;

};




EFF3D_END



#endif