<html>
 <body>
  <h1>Book shop</h1><hr/>
  <p>The simple project for simulating the book selling/purchasing with REST API</p>
  <hr/>
  <h2>Bank API</h2>
  <p>Bank API has the following function scripts:</p>
  <ul>
   <li>(POST)/api/create_account     -- variables: email, pwd</li>
   <li>(POST)/api/log_in             -- variables: email, pwd</li>
   <li>(POST)/api/auth               -- cookies: id, hash</li>
   <li>(POST)/api/deposit            -- cookies: id, hash; variables: ammount</li>
   <li>(POST)/api/transfer           -- cookies: id, hash, variables: id, ammount</li>
   <li>(GET)/api/get_balance         -- variables: id</li>
  </ul>
  <hr/>
  <h2>Shop API</h2>
  <ul>
   <li>(POST)/api/add_book           -- cookies: id, hash; variables: title, price</li>
   <li>(POST)/api/buy_book           -- cookies: id, hash; variables: book_id, bank_id, bank_pwd</li>
   <li>(GET)/api/list_books          -- response: array of books(id, seller_id, seller_bank_id, price, title)</li>
   <li>(GET)/api/list_owned_books    -- cookies: id, hash; response: array of books(id, seller_id, seller_bank_id, price, title)</li>
   <li>(GET)/api/book_info:id        -- response: seller_id, seller_bank_id, price, title</li>
  </ul>
 </body>
</html>
