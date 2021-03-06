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

#ifndef __itkFastMarchingNumberOfElementsStoppingCriterion_h
#define __itkFastMarchingNumberOfElementsStoppingCriterion_h

#include "itkFastMarchingStoppingCriterionBase.h"
#include "itkObjectFactory.h"

namespace itk
{
  /**
    \class FastMarchingNumberOfElementsStoppingCriterion
    \brief Stopping Criterion is verified when Current Number of Elements is equal
    to or greater than the provided Target Number Of Elements.

    \note For itk::Image, one element is one pixel. So the number of elements is directly
    linked to the physical size of the object, i.e.
    \f$ PhysicalSize = TargetNumberOfElements \cdot \prod_{i=1}{dim} Spacing_{i} \f$

    \note For itk::QuadEdgeMesh, one element is one vertex.

    \ingroup ITKFastMarching
    */
  template< class TInput, class TOutput >
  class FastMarchingNumberOfElementsStoppingCriterion :
      public FastMarchingStoppingCriterionBase< TInput, TOutput >
    {
  public:
    typedef FastMarchingNumberOfElementsStoppingCriterion         Self;
    typedef FastMarchingStoppingCriterionBase< TInput, TOutput >  Superclass;
    typedef SmartPointer< Self >                                  Pointer;
    typedef SmartPointer< const Self >                            ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(FastMarchingNumberOfElementsStoppingCriterion,
                 FastMarchingStoppingCriterionBase );

    typedef typename Superclass::OutputPixelType  OutputPixelType;
    typedef typename Superclass::NodeType         NodeType;

    /** Get/set the threshold used by the stopping criteria. */
    itkSetMacro( TargetNumberOfElements, IdentifierType );
    itkGetMacro( TargetNumberOfElements, IdentifierType );

    bool IsSatisfied() const
      {
      return ( this->m_CurrentNumberOfElements >= this->m_TargetNumberOfElements );
      }

    std::string GetDescription() const
      {
      return "Current Number of Elements >= Target Number of Elements";
      }

  protected:
    FastMarchingNumberOfElementsStoppingCriterion() : Superclass(),
      m_CurrentNumberOfElements( NumericTraits< IdentifierType >::Zero ),
      m_TargetNumberOfElements( NumericTraits< IdentifierType >::Zero )
    {}

    ~FastMarchingNumberOfElementsStoppingCriterion() {}

    IdentifierType  m_CurrentNumberOfElements;
    IdentifierType  m_TargetNumberOfElements;

    void SetCurrentNode( const NodeType& )
      {
      ++this->m_CurrentNumberOfElements;
      }

    void Reset()
      {
      this->m_CurrentNumberOfElements = NumericTraits< IdentifierType >::Zero;
      }

  private:
    FastMarchingNumberOfElementsStoppingCriterion( const Self& );
    void operator = ( const Self& );
    };

}
#endif // __itkFastMarchingNumberOfElementsStoppingCriterion_h
