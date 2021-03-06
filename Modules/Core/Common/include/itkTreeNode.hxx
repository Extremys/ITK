/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkTreeNode_hxx
#define __itkTreeNode_hxx

#include "itkTreeNode.h"
#include <cstring>

namespace itk
{
/** Constructor */
template< class TValueType >
TreeNode< TValueType >
::TreeNode():m_Parent(NULL)
{}

/** Destructor */
template< class TValueType >
TreeNode< TValueType >
::~TreeNode()
{
  if ( m_Parent )
    {
    m_Parent->Remove(this);
    }

  const ChildIdentifier numberOfChildren = static_cast< ChildIdentifier >( m_Children.size() );

  for ( ChildIdentifier i = numberOfChildren; i > 0; i-- )
    {
    m_Children[i - 1]->SetParent(NULL);
    }
  m_Children.clear();
  m_Parent = NULL;
  m_Data = 0;
}

/** Return the parent node */
template< class TValueType >
TreeNode< TValueType > *
TreeNode< TValueType >
::GetParent() const
{
  return m_Parent;
}

/** Get a child */
template< class TValueType >
TreeNode< TValueType > *
TreeNode< TValueType >
::GetChild(ChildIdentifier number) const
{
  const ChildIdentifier numberOfChildren = static_cast< ChildIdentifier >( m_Children.size() );

  if ( number < numberOfChildren )
    {
    return m_Children[number];
    }
  else
    {
    return NULL;
    }
}

/** Set the value of a node */
template< class TValueType >
TValueType
TreeNode< TValueType >
::Set(const TValueType data)
{
  TValueType help = m_Data;

  m_Data = data;
  return help;
}

/** Get the data of node */
template< class TValueType >
const TValueType &
TreeNode< TValueType >
::Get() const
{
  return m_Data;
}

/** Return true if has a parent */
template< class TValueType >
bool
TreeNode< TValueType >
::HasParent() const
{
  return ( m_Parent ) ? true : false;
}

/** Set the parent node */
template< class TValueType >
void
TreeNode< TValueType >
::SetParent(TreeNode< TValueType > *node)
{
  //keep ourself alive just a bit longer
  Pointer ourself = this;

  if ( m_Parent != NULL )
    {
    m_Parent->Remove(this);
    }
  m_Parent = node;
}

/** Return true if the node has children */
template< class TValueType >
bool
TreeNode< TValueType >
::HasChildren() const
{
  return ( m_Children.size() > 0 ) ? true : false;
}

/** Return the number of children */
template< class TValueType >
typename TreeNode< TValueType >::ChildIdentifier
TreeNode< TValueType >
::CountChildren() const
{
  return static_cast< ChildIdentifier >( m_Children.size() );
}

/** Remove a child node from the current node */
template< class TValueType >
bool
TreeNode< TValueType >
::Remove(Self *n)
{
  typename std::vector< Pointer >::iterator pos;
  pos = std::find(m_Children.begin(), m_Children.end(), n);
  if ( pos != m_Children.end() )
    {
    //keep node alive just a bit longer
    Pointer position = n;
    m_Children.erase(pos);
    n->SetParent(NULL);
    return true;
    }
  return false;
}

/** Replace a child by a new one */
template< class TValueType >
bool
TreeNode< TValueType >
::ReplaceChild(Self *oldChild, Self *newChild)
{
  const ChildIdentifier numberOfChildren = static_cast< ChildIdentifier >( m_Children.size() );

  for ( ChildIdentifier i = 0; i < numberOfChildren; i++ )
    {
    if ( m_Children[i] == oldChild )
      {
      m_Children[i] = newChild;
      return true;
      }
    }
  return false;
}

/** Return the child position given a node */
template< class TValueType >
OffsetValueType
TreeNode< TValueType >
::ChildPosition(const Self *node) const
{
  const ChildIdentifier numberOfChildren = static_cast< ChildIdentifier >( m_Children.size() );

  for ( ChildIdentifier i = 0; i < numberOfChildren; i++ )
    {
    if ( m_Children[i] == node )
      {
      return i;
      }
    }
  return -1;
}

/** Return the child position given an element, the first child found. */
template< class TValueType >
typename TreeNode< TValueType >::ChildIdentifier
TreeNode< TValueType >
::ChildPosition(TValueType element) const
{
  const ChildIdentifier numberOfChildren = static_cast< ChildIdentifier >( m_Children.size() );

  for ( ChildIdentifier i = 0; i < numberOfChildren; i++ )
    {
    if ( m_Children[i]->Get() == element )
      {
      return i;
      }
    }
  return -1;
}

/** Add a child node */
template< class TValueType >
void
TreeNode< TValueType >
::AddChild(Self *node)
{
  Pointer nodeKeepAlive = node;

  node->SetParent(this);
  m_Children.push_back(node);
}

/** Add a child at a specific position in the children list */
template< class TValueType >
void
TreeNode< TValueType >
::AddChild(ChildIdentifier number, Self *node)
{
  const ChildIdentifier numberOfChildren = static_cast< ChildIdentifier >( m_Children.size() );
  ChildIdentifier childId = static_cast<ChildIdentifier>( number );

  if ( childId > numberOfChildren )
    {
    m_Children.resize(childId);
    for ( ChildIdentifier i = numberOfChildren; i <= childId; i++ )
      {
      m_Children[i] = NULL;
      }
    m_Children[number] = node;
    return;
    }

  m_Children[number] = node;
}

/** Get the number of children given a name and a depth */
template< class TValueType >
typename TreeNode< TValueType >::ChildIdentifier
TreeNode< TValueType >
::GetNumberOfChildren(unsigned int depth, char *name) const
{
  typename ChildrenListType::const_iterator it = m_Children.begin();
  typename ChildrenListType::const_iterator itEnd = m_Children.end();

  ChildIdentifier cnt = 0;
  while ( it != itEnd )
    {
    if ( name == NULL || strstr(typeid( **it ).name(), name) )
      {
      cnt++;
      }
    it++;
    }

  it = m_Children.begin();
  itEnd = m_Children.end();
  if ( depth > 0 )
    {
    while ( it != itEnd )
      {
      cnt += ( *it )->GetNumberOfChildren(depth - 1, name);
      it++;
      }
    }

  return cnt;
}

/** Get children given a name and a depth */
#if !defined( CABLE_CONFIGURATION )
template< class TValueType >
typename TreeNode< TValueType >::ChildrenListType *
TreeNode< TValueType >
::GetChildren(unsigned int depth, char *name) const
{
  ChildrenListType *children = new ChildrenListType;

  typename ChildrenListType::const_iterator childrenListIt =
    m_Children.begin();
  typename ChildrenListType::const_iterator childrenListEnd =
    m_Children.end();

  while ( childrenListIt != childrenListEnd )
    {
    if ( name == NULL || strstr(typeid( **childrenListIt ).name(), name) )
      {
      children->push_back(*childrenListIt);
      }
    if ( depth > 0 )
      {
      ChildrenListType *nextchildren = ( **childrenListIt ).GetChildren(depth - 1,
                                                                        name);
      // Add the child to the current list
      typename ChildrenListType::const_iterator nextIt = nextchildren->begin();
      while ( nextIt != nextchildren->end() )
        {
        children->push_back(*nextIt);
        nextIt++;
        }
      delete nextchildren;
      }
    childrenListIt++;
    }

  return children;
}

#endif
} // namespace itk

#endif
