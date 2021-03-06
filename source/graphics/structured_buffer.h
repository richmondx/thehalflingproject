/**
 * Copyright 2010 Intel Corporation
 * All Rights Reserved
 *
 * Permission is granted to use, copy, distribute and prepare derivative works of this
 * software for any purpose and without fee, provided, that the above copyright notice
 * and this statement appear in all copies.  Intel makes no representations about the
 * suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
 * INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
 * INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
 * INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
 * assume any responsibility for any errors which may appear in this software nor any
 * responsibility to update it.
 */

/**
 * Modified for use in The Halfling Project - A Graphics Engine and Projects
 * The Halfling Project is the legal property of Adrian Astley
 * Copyright Adrian Astley 2013 - 2014
 */

#pragma once

#include "graphics/d3d_util.h"

#include <d3d11.h>
#include <vector>


namespace Graphics {

// NOTE: Ensure that T is exactly the same size/layout as the shader structure!
template <typename T>
class StructuredBuffer {
public:
	// Construct a structured buffer
	StructuredBuffer(ID3D11Device *d3dDevice, int numElements,
	                 UINT bindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
	                 bool dynamic = false);
	~StructuredBuffer();

private:
	int m_numElements;
	ID3D11Buffer *mBuffer;
	ID3D11ShaderResourceView *m_shaderResource;
	ID3D11UnorderedAccessView *m_unorderedAccess;

public:
	inline int NumElements() { return m_numElements; }
	inline ID3D11Buffer *GetBuffer() { return mBuffer; }
	inline ID3D11UnorderedAccessView *GetUnorderedAccess() { return m_unorderedAccess; }
	inline ID3D11ShaderResourceView *GetShaderResource() { return m_shaderResource; }

	// Only valid for dynamic buffers
	// TODO: Support NOOVERWRITE ring buffer?
	T *MapDiscard(ID3D11DeviceContext *d3dDeviceContext);
	void Unmap(ID3D11DeviceContext *d3dDeviceContext);

private:
	// Not implemented
	StructuredBuffer(const StructuredBuffer &);
	StructuredBuffer &operator=(const StructuredBuffer &);
};


template <typename T>
StructuredBuffer<T>::StructuredBuffer(ID3D11Device *d3dDevice, int numElements, UINT bindFlags, bool dynamic)
		: m_numElements(numElements),
		  m_shaderResource(0),
		  m_unorderedAccess(0) {
	CD3D11_BUFFER_DESC desc(sizeof(T) * numElements, bindFlags,
	                        dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT,
	                        dynamic ? D3D11_CPU_ACCESS_WRITE : 0,
	                        D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
	                        sizeof(T));

	d3dDevice->CreateBuffer(&desc, 0, &mBuffer);

	if (bindFlags & D3D11_BIND_UNORDERED_ACCESS) {
		d3dDevice->CreateUnorderedAccessView(mBuffer, 0, &m_unorderedAccess);
	}

	if (bindFlags & D3D11_BIND_SHADER_RESOURCE) {
		d3dDevice->CreateShaderResourceView(mBuffer, 0, &m_shaderResource);
	}
}

template <typename T>
StructuredBuffer<T>::~StructuredBuffer() {
	ReleaseCOM(m_unorderedAccess);
	ReleaseCOM(m_shaderResource);
	ReleaseCOM(mBuffer);
}

template <typename T>
T *StructuredBuffer<T>::MapDiscard(ID3D11DeviceContext *d3dDeviceContext) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	d3dDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	return static_cast<T *>(mappedResource.pData);
}

template <typename T>
void StructuredBuffer<T>::Unmap(ID3D11DeviceContext *d3dDeviceContext) {
	d3dDeviceContext->Unmap(mBuffer, 0);
}


// TODO: Constant buffers

} // End of namespace Graphics
