#define VMA_IMPLEMENTATION
#include "Injector/Graphics/Vulkan/VkGpuBuffer.hpp"
#include "Injector/Exception/NullException.hpp"
#include "Injector/Exception/OutOfRangeException.hpp"
#include "Injector/Graphics/Vulkan/VkGpuBufferType.hpp"

namespace Injector
{
	bool VkGpuBuffer::isVkMappable(
		VmaMemoryUsage usage)
	{
		return
			usage == VMA_MEMORY_USAGE_CPU_ONLY ||
			usage == VMA_MEMORY_USAGE_CPU_TO_GPU ||
			usage == VMA_MEMORY_USAGE_GPU_TO_CPU;
	}

	VkGpuBuffer::VkGpuBuffer(
		VmaAllocator _allocator,
		vk::BufferUsageFlags usageFlags,
		VmaMemoryUsage usage,
		GpuBufferType type,
		size_t size) :
		GpuBuffer(type, size, isVkMappable(usage)),
		allocator(_allocator)
	{
		if(!_allocator)
		{
			throw NullException(
				THIS_FUNCTION_NAME,
				"allocator");
		}

		VkBufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.flags = 0;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = static_cast<VkBufferUsageFlags>(
			toVkGpuBufferType(type) |
			usageFlags);
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferCreateInfo.queueFamilyIndexCount = 0;
		bufferCreateInfo.pQueueFamilyIndices = nullptr;

		VmaAllocationCreateInfo allocationCreateInfo = {};
		allocationCreateInfo.flags = VMA_ALLOCATION_CREATE_WITHIN_BUDGET_BIT;
		allocationCreateInfo.usage = usage;
		// TODO: VMA_MEMORY_USAGE_GPU_LAZILY_ALLOCATED on mobiles

		VkBuffer bufferHandle;
		allocation = nullptr;

		auto result = vmaCreateBuffer(
			_allocator,
			&bufferCreateInfo,
			&allocationCreateInfo,
			&bufferHandle,
			&allocation,
			nullptr);

		if (result != VK_SUCCESS)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Failed to create buffer");
		}

		buffer = vk::Buffer(bufferHandle);
	}
	VkGpuBuffer::~VkGpuBuffer()
	{
		vmaDestroyBuffer(
			allocator,
			static_cast<VkBuffer>(buffer),
			allocation);
	}

	VmaAllocator VkGpuBuffer::getAllocator() const noexcept
	{
		return allocator;
	}
	vk::Buffer VkGpuBuffer::getBuffer() const noexcept
	{
		return buffer;
	}
	VmaAllocation VkGpuBuffer::getAllocation() const noexcept
	{
		return allocation;
	}

	void VkGpuBuffer::invalidate(
		size_t _size,
		size_t offset)
	{
		auto result = vmaInvalidateAllocation(
			allocator,
			allocation,
			offset,
			_size);

		if (result != VK_SUCCESS)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Failed to invalidate buffer");
		}
	}
	void VkGpuBuffer::flush(
		size_t _size,
		size_t offset)
	{
		auto result = vmaFlushAllocation(
			allocator,
			allocation,
			offset,
			_size);

		if (result != VK_SUCCESS)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Failed to flush buffer");
		}
	}

	void* VkGpuBuffer::map(
		GpuBufferAccess access)
	{
		if (!mappable)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is not mappable");
		}
		if (mapped)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is already mapped");
		}

		void* mappedData;

		auto result = vmaMapMemory(
			allocator,
			allocation,
			&mappedData);

		if (result != VK_SUCCESS)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Failed to map buffer");
		}

		if (access == GpuBufferAccess::ReadOnly ||
			access == GpuBufferAccess::ReadWrite)
		{
			invalidate(size, 0);
		}

		mapped = true;
		mapAccess = access;
		mapSize = size;
		mapOffset = 0;
		return mappedData;
	}
	void* VkGpuBuffer::map(
		GpuBufferAccess access,
		size_t _size,
		size_t offset)
	{
		if (!mappable)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is not mappable");
		}
		if (mapped)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is already mapped");
		}
		if (_size + offset > size)
		{
			throw OutOfRangeException(
				THIS_FUNCTION_NAME,
				static_cast<uint64_t>(_size + offset),
				static_cast<uint64_t>(size));
		}

		void* mappedData;

		auto result = vmaMapMemory(
			allocator,
			allocation,
			&mappedData);

		if (result != VK_SUCCESS)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Failed to map buffer");
		}

		if (access == GpuBufferAccess::ReadOnly ||
			access == GpuBufferAccess::ReadWrite)
		{
			invalidate(_size, offset);
		}

		mapped = true;
		mapAccess = access;
		mapSize = _size;
		mapOffset = offset;
		return mappedData;
	}
	void VkGpuBuffer::unmap()
	{
		if (!mapped)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is not mapped");
		}

		if (mapAccess == GpuBufferAccess::WriteOnly ||
			mapAccess == GpuBufferAccess::ReadWrite)
		{
			flush(mapSize, mapOffset);
		}

		vmaUnmapMemory(allocator, allocation);
		mapped = false;
	}

	void VkGpuBuffer::setData(
		const void* data,
		size_t _size)
	{
		if (!mappable)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is not mappable");
		}
		if (mapped)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is already mapped");
		}
		if(!data)
		{
			throw NullException(
				THIS_FUNCTION_NAME,
				"data");
		}
		if (_size > size)
		{
			throw OutOfRangeException(
				THIS_FUNCTION_NAME,
				static_cast<uint64_t>(_size),
				static_cast<uint64_t>(size));
		}

		void* mappedData;

		auto result = vmaMapMemory(
			allocator,
			allocation,
			&mappedData);

		if (result != VK_SUCCESS)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Failed to map buffer");
		}

		memcpy(
			mappedData,
			data,
			_size);

		flush(_size, 0);
		vmaUnmapMemory(allocator, allocation);
	}
	void VkGpuBuffer::setData(
		const void* data,
		size_t _size,
		size_t offset)
	{
		if (!mappable)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is not mappable");
		}
		if (mapped)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Buffer is already mapped");
		}
		if(!data)
		{
			throw NullException(
				THIS_FUNCTION_NAME,
				"data");
		}
		if (_size + offset > size)
		{
			throw OutOfRangeException(
				THIS_FUNCTION_NAME,
				static_cast<uint64_t>(_size + offset),
				static_cast<uint64_t>(size));
		}

		void* mappedData;

		auto result = vmaMapMemory(
			allocator,
			allocation,
			&mappedData);

		if (result != VK_SUCCESS)
		{
			throw Exception(
				THIS_FUNCTION_NAME,
				"Failed to map buffer");
		}

		auto castedData = static_cast<char*>(mappedData);

		memcpy(
			castedData + offset,
			data,
			_size);

		flush(_size, offset);
		vmaUnmapMemory(allocator, allocation);
	}
}
