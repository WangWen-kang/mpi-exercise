      subroutine reduce_vector( n, y, x )
c     ===============================
c  all variables should be explicitely declared !
      implicit none 
c
      integer           n
      real*8            y(*), x(*)
c
c  +-------------+----------------+------------------------------------+
c  |  parameter  |  input/output  |  description                       |
c  +-------------+----------------+------------------------------------+
c  |  n          |  input         |  global vectorlength               |
c  |  y          |  input         |  partial sum of global vector      |
c  |  x          |  output        |  local elements of full vector     |
c  +-------------+----------------+------------------------------------+
c  |                                                                   |
c  |  Purpose of this subroutine is to collect partial result vectors  |
c  |  from all processor and add them up to the global result.         |
c  |                                                                   |
c  +-------------------------------------------------------------------+
c  |  ruler                                                            |
c  |5 7 10   15   20   25   30   35   40   45   50   55   60   65   70 | 
c  ++-+--+----+----+----+----+----+----+----+----+----+----+----+----+-+
c
c  parameter constants
      integer           nprocmax
      parameter         (nprocmax = 128)
c  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
c  local variables
      integer           ierr, nproc, myid, status(MPI_STATUS_SIZE)
      integer           firstind(0:nprocmax), ip, nl, ia
      integer           recvcounts(0:nprocmax)

      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )

      call dist_index( n, firstind )
      do ip = 0 , nproc-1
         recvcounts(ip) = firstind(ip+1) - firstind(ip)
      end do
      call MPI_REDUCE_SCATTER( y, x, recvcounts, MPI_DOUBLE_PRECISION, 
     :                    MPI_SUM, MPI_COMM_WORLD, ierr )
 
      return
      end
