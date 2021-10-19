package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.HouseDao;
import com.ssafy.dto.HouseDto;
import com.ssafy.util.DBUtil;

public class HouseDaoImpl implements HouseDao {
	private static HouseDao houseDao = new HouseDaoImpl();
	private DBUtil dbUtil;

	private HouseDaoImpl() {
		dbUtil = DBUtil.getInstance();
	}

	public static HouseDao getHouseDao() {
		return houseDao;
	}

//	@Override
//	public void register(HouseDto HouseDto) throws Exception {
//		Connection conn = null;
//		PreparedStatement pstmt = null;
//		try {
//			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
//			StringBuilder sql = new StringBuilder();
//			sql.append("insert into houseinfo (no, dong, AptName, code, buildYear, jibun, lat, lng, img)\n"
//					+ "values (?,?,?,?,?,?,?,?,?)");
//			pstmt = conn.prepareStatement(sql.toString());
//			pstmt.setInt(1, HouseDto.getNo());
//			pstmt.setString(2, HouseDto.getDong());
//			pstmt.setString(3, HouseDto.getAptName());
//			pstmt.setString(4, HouseDto.getCode());
//			pstmt.setString(5, HouseDto.getBuildYear());
//			pstmt.setString(6, HouseDto.getJibun());
//			pstmt.setString(7, HouseDto.getLat());
//			pstmt.setString(8, HouseDto.getLng());
//			pstmt.setString(9, HouseDto.getImg());
//
//			int cnt = pstmt.executeUpdate();
//			System.out.println(cnt + "개 삽입성공!!!");
//		} catch (SQLException e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				if (pstmt != null)
//					pstmt.close();
//				if (conn != null)
//					conn.close();
//			} catch (SQLException e) {
//				e.printStackTrace();
//			}
//		}
//	}

//	@Override
//	public HouseDto searchById(int houseNo) {
//		Connection conn = null;
//		PreparedStatement pstmt = null;
//		ResultSet rs = null;
//		HouseDto houseDto = null;
//
//		try {
//			conn = DBUtil.getConnection();
//			String sql = "select no, dong, AptName, code, buildYear, jibun, lat, lng, img " + "where no = ?";
//			pstmt = conn.prepareStatement(sql);
//			pstmt.setInt(1, houseNo);
//			rs = pstmt.executeQuery();
//			if (rs.next()) {
//				houseDto = new HouseDto(rs.getInt("no"), rs.getString("dong"), rs.getString("AptName"),
//						rs.getString("code"), rs.getString("buildYear"), rs.getString("jibun"), rs.getString("lat"),
//						rs.getString("lng"), rs.getString("img"));
//			}
//
//		} catch (SQLException e) {
//			e.printStackTrace();
//		} finally {
//			DBClose.close(rs, pstmt, conn);
//		}
//		return houseDto;
//	}

	@Override
	public ArrayList<HouseDto> searchByName(String sido, String gugun, String dong, String name) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		ArrayList<HouseDto> houseDtoList = new ArrayList<HouseDto>();
		System.out.println(sido + " " + gugun + " " + dong + " " + name);
		try {
			conn = dbUtil.getConnection();
			String sql = "select *\n" + "from houseinfo"
					+ " where sido like ? and gugun like ? and dong like ? and apt_name like ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, '%' + sido + '%');
			pstmt.setString(2, '%' + gugun + '%');
			pstmt.setString(3, '%' + dong + '%');
			pstmt.setString(4, '%' + name + '%');
			rs = pstmt.executeQuery();

			while (rs.next()) {
				HouseDto houseDto = new HouseDto(rs.getInt("no"), rs.getString("sido"), rs.getString("gugun"),
						rs.getString("dong"), rs.getString("dong_code"), rs.getString("apt_name"),
						rs.getString("build_year"), rs.getString("jibun"), rs.getString("lat"), rs.getString("lng"),
						rs.getString("img"));

				houseDtoList.add(houseDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return houseDtoList;
	}

//	@Override
//	public List<HouseDto> searchAll() {
//		Connection conn = null;
//		PreparedStatement pstmt = null;
//		ResultSet rs = null;
//		List<HouseDto> houseDtoList = new ArrayList<HouseDto>();
//		try {
//			conn = DBUtil.getConnection();
//			String sql = "select no, dong, AptName, code, buildYear, jibun, lat, lng, img " + "from houseinfo";
//			pstmt = conn.prepareStatement(sql);
//
//			rs = pstmt.executeQuery();
//			while (rs.next()) {
//				HouseDto houseDto = new HouseDto(rs.getInt("no"), rs.getString("dong"), rs.getString("AptName"),
//						rs.getString("code"), rs.getString("buildYear"), rs.getString("jibun"), rs.getString("lat"),
//						rs.getString("lng"), rs.getString("img"));
//				houseDtoList.add(houseDto);
//			}
//
//		} catch (SQLException e) {
//			e.printStackTrace();
//		} finally {
//			DBClose.close(rs, pstmt, conn);
//		}
//		return houseDtoList;
//	}

//	@Override
//	public void deleteHouse(String houseNo) {
//		Connection conn = null;
//		PreparedStatement pstmt = null;
//		try {
//			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
//			StringBuilder sql = new StringBuilder();
//			sql.append("delete from houseinfo\n" + "where no = ?");
//			pstmt = conn.prepareStatement(sql.toString());
//
//			pstmt.setString(1, houseNo);
//			pstmt.executeUpdate();
//			System.out.println("삭제 성공!!!");
//		} catch (SQLException e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				if (pstmt != null)
//					pstmt.close();
//				if (conn != null)
//					conn.close();
//			} catch (SQLException e) {
//				e.printStackTrace();
//			}
//		}
//	}

	@Override
	public List<HouseDto> searchByDong(String houseDong) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void register(HouseDto houseDto) {
		// TODO Auto-generated method stub

	}

	@Override
	public HouseDto searchById(int houseNo) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public List<HouseDto> searchAll() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void deleteHouse(String houseNo) {
		// TODO Auto-generated method stub

	}

	public String[] getDongLatLng(String sido, String gugun, String dong) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		String[] latlng = null;
		try {
			conn = dbUtil.getConnection();
			String sql = "select lat, lng\n" + "from dongcode" + " where city like ? and gugun like ? and dong like ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, '%' + sido + '%');
			pstmt.setString(2, '%' + gugun + '%');
			pstmt.setString(3, '%' + dong + '%');
			rs = pstmt.executeQuery();
			rs.next();
			latlng = new String[] { rs.getString("lat"), rs.getString("lng") };

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return latlng;
	}

}
