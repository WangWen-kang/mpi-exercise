      program ritz_dist1
c     ==================
c
c  +-------------------------------------------------------------------+
c  |                                                                   |
c  |  This program determines the largest eigenvalue of a matrix using |
c  |  the method of Raleigh and Ritz.                                   |
c  |                                                                   |
c  |  Distributed version 1 - column blocks                            |
c  |                                                                   |
c  +-------------------------------------------------------------------+
c  |  ruler                                                            |
c  |5 7 10   15   20   25   30   35   40   45   50   55   60   65   70 | 
c  ++-+--+----+----+----+----+----+----+----+----+----+----+----+----+-+
c
c  all variables should be explicitely declared !
      implicit none 
c  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
      integer           ierr, nproc, myid
c  parameter constants
      integer           nmax, nprocmax
      parameter         (nmax = 8*1024, nprocmax = 128)
c  local variables
      integer           firstind(0:nprocmax), counts(0:nprocmax),
     :                  dspls(0:nprocmax), 
     :                  i, j, n, nl, nloop, istop
      real*8            a(nmax * nmax), al(nmax*nmax),
     :                  x(nmax), y(nmax), 
     :                  flops, tcpu, tdelta, evn, evo, res
      real*8            t1, t2, t3, t4, t5, tmv, tcl, tte, tsc
c  initialize MPI, get nproc (# of procs) and myid (local proc. id)
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
c  om proc 0 initialize matrix
      if (myid.eq.0) then
c  get matrix size
         print 6000
         read *, n
         if  (n .le. 0  .or.  n .gt. nmax)  stop 'n out of range'
         nloop = 1000
c  initialize global matrix
         print 6100
         do  j = 1, n
            do  i = 1 , j - 1
               a(i + n * (j - 1)) = a(j + n * (i - 1))
            end do
            do  i = j, n
               a(i + n * (j - 1)) = (dble(i) + dble(j))/dble(i)
            end do
         end do
      end if
      call MPI_BARRIER( MPI_COMM_WORLD, ierr )
      call MPI_BCAST( nloop, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr )
c  distribute global matrix
      call MPI_BCAST( n, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr )
      call dist_index( n, firstind )
      do i = 0 , nproc-1
         counts(i) = n*(firstind(i+1)-firstind(i))
         dspls(i) = n * firstind(i)
      end do
      call MPI_SCATTERV( a,counts,dspls,MPI_DOUBLE_PRECISION
     :    , al, counts(myid), MPI_DOUBLE_PRECISION
     :    , 0, MPI_COMM_WORLD, ierr )
 
      nl = firstind(myid+1) - firstind(myid)
!      call dist_matrix_colblock( n, n, a, al )
c--------------------------------------------------------------
c  Raleigh Ritz algoritm
c--------------------------------------------------------------
c  initialize start vector x
      do i = 1 , nl
         x(i) = 1.0
      end do
      tmv = 0.0
      tcl = 0.0
      tte = 0.0
      tsc = 0.0
      tcpu = MPI_WTIME()
  
      evo = x(1)
      istop = 0
      do i = 0 , nproc-1
         counts(i) = firstind(i+1)-firstind(i)
      end do
      do  i = 1, nloop
         t1 = MPI_WTIME()
         CALL MV (n, nl, al, x, y)
         t2 = MPI_WTIME()
         call MPI_BARRIER( MPI_COMM_WORLD, ierr )
         call MPI_Reduce_scatter(y, x, counts
     :                         , MPI_DOUBLE_PRECISION, MPI_SUM  
     :                         , MPI_COMM_WORLD, ierr)
!         call reduce_vector(n,y,x)
         t3 = MPI_WTIME()
         if (myid.eq.0) then
            evn = x(1)
            res = (evn - evo) / evn
            write(6,*)i,evn,res
            if (abs(res).le.1.0e-12) istop = 1
         end if
         call MPI_BCAST( evn, 1, MPI_DOUBLE_PRECISION, 0, 
     :                   MPI_COMM_WORLD, ierr )
         call MPI_BCAST( istop, 1, MPI_INTEGER, 0, 
     :                   MPI_COMM_WORLD, ierr )
         evo = evn
         t4 = MPI_WTIME()
         if (istop.eq.1) go to 1
         do j = 1 , nl
            x(j) = x(j) / evn
         end do
         t5 = MPI_WTIME()
         tmv = tmv + t2 - t1
         tcl = tcl + t3 - t2
         tte = tte + t4 - t3
         tsc = tsc + t5 - t4
      end do
 1    continue
      tcpu = MPI_WTIME() - tcpu
c  print results
      if (myid.eq.0) then
         write(6,6400) tmv, tcl, tte, tsc
         if (tcpu .gt. 0.0) then
            flops = 1e-6 * float(2 * i * n * n) / tcpu
         else
            flops = 0.0
         endif
         print 6300, flops, tcpu
      end if
      call MPI_FINALIZE( ierr )
c  format statements
 6000 format(' ', /,
     :       ' Raleigh-Ritz running ...', //,
     :       ' N = ?')
 6100 format(' ', /,
     :       ' Initializing data ...')
 6200 format(' Raleigh-Ritz ...', /)
 6300 format(' MFlop/s ............................ : ', f12.4, /,
     :       ' CPU seconds ........................ : ', e12.4, /)
 6400 format('        mv           com        test       copy'/4e12.5/)
c  end of program ritz_dist_I
      end

