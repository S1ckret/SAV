#include "CountingSort.h"

#include "Log.h"

extern bool dirtyFlag;

CountingSort::CountingSort( Array &array, std::shared_ptr<IDataRenderer> &dataRednderer )
	: ISort( array, dataRednderer ) {
	m_name = "Counting Sort";
	LOG_TRACE( m_name, " has created." );
}

void CountingSort::Begin()
{
	dirtyFlag = 0;
	int max = m_array.GetMaxValue( );
	unsigned size = m_array.Size( );
	LOG_TRACE( "Max value: {0}", max );
	LOG_TRACE( "Size: {0}", size );
	max++;

	unsigned int * count_array = (unsigned int*)malloc(sizeof(unsigned int) * max);
	int * sorted_array = (int*)malloc(sizeof(int) * size);
	memset(count_array, 0, sizeof(unsigned int) * max);
	memset(sorted_array, 0, sizeof(int) * size );

	LOG_TRACE( "Counting..." );
	for (unsigned int i = 0; i < size; i++)
	{
		m_dataRenderer->RenderIterator( "i", i, 0xff0000 );
		count_array[m_array[i].Data()]++;
	}
	m_dataRenderer->RemoveIterator( "i" );

	LOG_TRACE( "Creating indicies..." );
	for (unsigned int i = 1; i < max; i++)
	{
		count_array[i] += count_array[i - 1];
	}

	LOG_TRACE( "Creating sorted array in auxiliary array..." );
	for (unsigned int i = 0; i < size; i++)
	{
		m_dataRenderer->RenderIterator( "i", i, 0x00ff00 );
		unsigned int index = count_array[m_array[i].Data( )] - 1;
		--count_array[m_array[i].Data( )];
		sorted_array[index] = m_array[i].Data( );

	}
	m_dataRenderer->RemoveIterator( "i" );

	LOG_TRACE( "Copying sorted array to main array..." );
	for ( unsigned int i = 0; i < size; i++ ) {
		m_dataRenderer->RenderIterator( "i", i, 0x0000ff );
		m_array[i] = sorted_array[i];
	}
	m_dataRenderer->RemoveIterator( "i" );

	
	free( sorted_array );
	free( count_array );
	m_dataRenderer->DisplaySorted( );
	LOG_INFO("I am sorted !");
	dirtyFlag = 1;
}

