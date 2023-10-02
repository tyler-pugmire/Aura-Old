#pragma once

#include <stdint.h>
#include <atomic>

namespace Aura
{
    class RefCounted
    {
    public:
        void IncRefCount() const
        {
            ++m_refCount;
        }

        void DecRefCount() const
        {
            --m_refCount;
        }

        uint32_t GetRefCount() const { return m_refCount.load(); }
    private:
        mutable std::atomic<uint32_t> m_refCount = 0;
    };

    namespace RefUtils 
    {
        void AddToLiveReferences(void* instance);
        void RemoveFromLiveReferences(void* instance);
        bool IsLive(void* instance);
    }

    template<typename T>
    class Ref
    {
    public:
        Ref() : m_instance(nullptr) {}
        Ref(std::nullptr_t n) : m_instance(nullptr) {}

        Ref(T* instance) : m_instance(instance)
        {
            static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");
            IncRef();
        }

        template<typename T2>
        Ref(const Ref<T2>& other)
        {
            m_instance = (T*)other.m_instance;
            IncRef();
        }

        template<typename T2>
        Ref(Ref<T2>&& other)
        {
            m_instance = (T*)other.m_instance;
            other.m_instance = nullptr;
        }

        ~Ref()
        {
            DecRef();
        }

        Ref(const Ref<T>& other)
            : m_instance(other.m_instance)
        {
            IncRef();
        }

        Ref& operator=(std::nullptr_t)
        {
            DecRef();
            m_instance = nullptr;
            return *this;
        }

        Ref& operator=(const Ref<T>& other)
        {
            other.IncRef();
            DecRef();

            m_instance = other.m_instance;
            return *this;
        }

        template<typename T2>
        Ref& operator=(const Ref<T2>& other)
        {
            other.IncRef();
            DecRef();

            m_instance = other.m_instance;
            return *this;
        }

        template<typename T2>
        Ref& operator=(Ref<T2>&& other)
        {
            DecRef();

            m_instance = other.m_instance;
            other.m_instance = nullptr;
            return *this;
        }

        operator bool() { return m_instance != nullptr; }
        operator bool() const { return m_instance != nullptr; }

        T* operator->() { return m_instance; }
        const T* operator->() const { return m_instance; }

        T& operator*() { return *m_instance; }
        const T& operator*() const { return *m_instance; }

        T* Raw() { return  m_instance; }
        const T* Raw() const { return  m_instance; }

        void Reset(T* instance = nullptr)
        {
            DecRef();
            m_instance = instance;
        }

        template<typename T2>
        Ref<T2> As() const
        {
            return Ref<T2>(*this);
        }

        template<typename... Args>
        static Ref<T> Create(Args&&... args)
        {
            return Ref<T>(new T(std::forward<Args>(args)...));
        }

        bool operator==(const Ref<T>& other) const
        {
            return m_instance == other.m_instance;
        }

        bool operator!=(const Ref<T>& other) const
        {
            return !(*this == other);
        }

        bool EqualsObject(const Ref<T>& other)
        {
            if (!m_instance || !other.m_instance)
                return false;

            return *m_instance == *other.m_instance;
        }
    private:
        void IncRef() const
        {
            if (m_instance)
            {
                m_instance->IncRefCount();
                RefUtils::AddToLiveReferences((void*)m_instance);
            }
        }

        void DecRef() const
        {
            if (m_instance)
            {
                m_instance->DecRefCount();
                if (m_instance->GetRefCount() == 0)
                {
                    delete m_instance;
                    RefUtils::RemoveFromLiveReferences((void*)m_instance);
                    m_instance = nullptr;
                }
            }
        }

        template<class T2>
        friend class Ref;
        mutable T* m_instance;
    };

    template<typename T>
    class WeakRef
    {
    public:
        WeakRef() = default;

        WeakRef(Ref<T> ref)
        {
            m_instance = ref.Raw();
        }

        WeakRef(T* instance)
        {
            m_instance = instance;
        }

        T* operator->() { return m_instance; }
        const T* operator->() const { return m_instance; }

        T& operator*() { return *m_instance; }
        const T& operator*() const { return *m_instance; }

        bool IsValid() const { return m_instance ? RefUtils::IsLive(m_instance) : false; }
        operator bool() const { return IsValid(); }
    private:
        T* m_instance = nullptr;
    };
}