/*
 * [PSXFunkin-n00b]
 *   HashMap.h
 * Author(s): Regan Green
 * Date: 03/17/2022

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <stdlib.h>

namespace HashMap
{
	// Hash map class
	struct Node
	{
		uint32_t hash, value;
		Node *next;
	};

	template<int bucket_bits>
	class HashMap
	{
		private:
			// Buckets
			constexpr static int bucket_len = (1 << bucket_bits);
			constexpr static int bucket_and = bucket_len - 1;

			Node *bucket[bucket_len] = {};

		public:
			~HashMap()
			{
				// Free buckets
				Clear();
			}

			void Clear()
			{
				// Free buckets
				for (auto &i : bucket)
				{
					for (Node *node = i; node != nullptr;)
					{
						Node *next = node->next;
						delete node;
						node = next;
					}
					i = nullptr;
				}
			}

			uint32_t Get(uint32_t hash)
			{
				// Traverse matching bucket
				for (Node *node = bucket[hash & bucket_and]; node != nullptr; node = node->next)
				{
					// Return matching hash's value
					if (node->hash == hash)
						return node->value;
				}
				return 0;
			}

			void Set(uint32_t hash, uint32_t value)
			{
				// Traverse matching bucket
				for (Node *node = bucket[hash & bucket_and]; node != nullptr; node = node->next)
				{
					// Apply to matching hash's value
					if (node->hash == hash)
					{
						node->value = value;
						return;
					}
				}

				// Create new node
				Node *node = new Node;
				node->hash = hash;
				node->value = value;
				if ((node->next = bucket[hash & bucket_and]) == nullptr)
					bucket[hash & bucket_and] = node;
			}
	};

	// Tree hash map class
	template<int bucket_bits> class TreeHashMap;

	template<int bucket_bits>
	struct TreeNode
	{
		TreeHashMap<bucket_bits> hashmap;
		uint32_t hash, value;
		TreeNode<bucket_bits> *next;
	};

	template<int bucket_bits>
	class TreeHashMap
	{
		private:
			// Buckets
			constexpr static int bucket_len = (1 << bucket_bits);
			constexpr static int bucket_and = bucket_len - 1;

			TreeNode<bucket_bits> *bucket[bucket_len] = {};

		public:
			~TreeHashMap()
			{
				// Free buckets
				Clear();
			}

			void Clear()
			{
				// Free buckets
				for (auto &i : bucket)
				{
					for (TreeNode<bucket_bits> *node = i; node != nullptr;)
					{
						TreeNode<bucket_bits> *next = node->next;
						delete node;
						node = next;
					}
					i = nullptr;
				}
			}

			uint32_t Get(const uint32_t *hash)
			{
				// Traverse matching bucket
				for (TreeNode<bucket_bits> *node = bucket[hash[0] & bucket_and]; node != nullptr; node = node->next)
				{
					// Traverse matching node
					if (node->hash == hash[0])
					{
						if (hash[1] != 0)
							return node->hashmap.Get(hash + 1);
						else
							return node->value;
					}
				}
				return 0;
			}

			void Set(const uint32_t *hash, uint32_t value)
			{
				// Traverse matching bucket
				for (TreeNode<bucket_bits> *node = bucket[hash[0] & bucket_and]; node != nullptr; node = node->next)
				{
					// Traverse matching node
					if (node->hash == hash[0])
					{
						if (hash[1] != 0)
						{
							node->hashmap.Set(hash + 1, value);
							return;
						}
						else
						{
							node->value = value;
							return;
						}
					}
				}

				// Create new node
				TreeNode<bucket_bits> *node = new TreeNode<bucket_bits>;
				node->hash = hash[0];
				if (hash[1] != 0)
				{
					node->hashmap.Set(hash + 1, value);
					node->value = 0;
				}
				else
				{
					node->value = value;
				}
				if ((node->next = bucket[hash[0] & bucket_and]) == nullptr)
					bucket[hash[0] & bucket_and] = node;
			}
	};
}
