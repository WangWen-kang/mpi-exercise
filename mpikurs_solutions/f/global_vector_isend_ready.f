      subroutine global_vector( n, y, x )
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
c  |  y          |  input         |  local part of global vector       |
c  |  x          |  output        |  reconstructed global vector       |
c  +-------------+----------------+------------------------------------+
c  |                                                                   |
c  |  Purpose of this subroutine is to collect the local parts of a    |
c  |  vector from all processor and combine them to the global vector. |
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
      integer           firstind(0:nprocmax), req(0:nprocmax),
     :                  ip, nl, ia, i


      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )

      call dist_index( n, firstind )
      nl = firstind(myid+1) - firstind(myid)
      do ip = 0 , nproc-1
         call MPI_ISSEND( y, nl, MPI_DOUBLE_PRECISION, ip, 0,
     :                     MPI_COMM_WORLD, req(ip), ierr )
      end do
      do ip = 0 , nproc-1
         nl = firstind(ip+1) - firstind(ip)
         ia = firstind(ip)+1
         call MPI_RECV( x(ia), nl , MPI_DOUBLE_PRECISION, ip, 0, 
     :                  MPI_COMM_WORLD, MPI_STATUS_IGNORE, ierr )
      end do
      do ip = 0 , nproc-1
         call MPI_WAIT( req(ip), MPI_STATUS_IGNORE, ierr ) 
      end do
 
      return
      end
