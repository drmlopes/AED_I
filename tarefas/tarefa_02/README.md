# Djonatan Roque Moreira Lopes

# 143. Reorder List

You are given the head of a singly linked-list. The list can be represented as:

> L0 → L1 → … → Ln - 1 → Ln

Reorder the list to be on the following form:

> L0 → Ln → L1 → Ln - 1 → L2 → Ln - 2 → …

You may not modify the values in the list's nodes. Only nodes themselves may be changed.

**Example 1:**

> **Input:** head = [1,2,3,4]
>
> **Output:** [1,4,2,3]

**Example 2:**

> **Input:** head = [1,2,3,4,5]
>
> **Output:** [1,5,2,4,3]

**Constraints:**

* The number of nodes in the list is in the range `[1, 5 * 10⁴]`
* `1 <= Node.val <= 1000`

**Test Cases:**

> **Case 1:**
>> **Input:** head = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24]
>>
>> **Output:** [1,24,2,23,3,22,4,21,5,20,6,19,7,18,8,17,9,16,10,15,11,14,12,13]